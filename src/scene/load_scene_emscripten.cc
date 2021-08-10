#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <boost/format.hpp>
#include "scene/load_scene_emscripten.h"
#include "button/labeled_button.h"
#include "button/button.h"
#include "scene/game_scene.h"
#include "word_loader/file_word_loader.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace cross_language_match {

#ifdef __EMSCRIPTEN__

// Emscripten exporting only works for C functions
// This function is intended to be exported with Emscripten so it can be invoked via Javascript to
// persist a web-loaded file in the local virtual filesystem (so it can be accessed in the game).
extern "C" {
void persist_file(char *contents_to_persist) {
  FILE *file = fopen(cross_language_match::kEmscriptenInputFilePath, "w+");
  fprintf(file, "%s", contents_to_persist);
  fclose(file);
}
}

EM_JS(
    void,
    load_file,
    (const char* loaded_file_name),
    {
      var input = document.createElement('input');
      input.type = 'file';
      input.onchange = e => {

        var fileBlob = e.target.files[0];

        if (fileBlob.length == 0) {
          return;
        }

        var reader = new FileReader();

        reader.onload = function() {
          var data_str = reader.result;
          Module.ccall('persist_file', null, ["string"], [data_str]);
          // Populate the passed in filename variable
          var lengthBytes = lengthBytesUTF8(fileBlob.name) + 1;
          stringToUTF8(fileBlob.name, loaded_file_name, lengthBytes);
        };

        reader.readAsText(fileBlob);

      };

      input.click();

    }
);

#endif

LoadSceneEmscripten::LoadSceneEmscripten(SDL_Renderer *renderer,
                                         SDL_Window *window,
                                         bool &global_quit,
                                         int screen_height,
                                         int screen_width)
    : Scene(renderer, window, global_quit),
      screen_height_(screen_height),
      screen_width_(screen_width) {

  if (TTF_Init() == -1) {
    throw std::runtime_error(
        boost::str(boost::format("SDL_ttf could not be initialized, error: %1%\n") % TTF_GetError())
    );
  }

  button_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", load_button_font_size_);
  if (button_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1%\n") % TTF_GetError()));
  }

  small_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", small_font_size_);
  if (small_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1%\n") % TTF_GetError()));
  }

}

LoadSceneEmscripten::~LoadSceneEmscripten() {

  TTF_CloseFont(button_font_);
  button_font_ = nullptr;

  TTF_CloseFont(small_font_);
  small_font_ = nullptr;

  TTF_Quit();

}

void LoadSceneEmscripten::RunPreLoop() {

  SDL_SetRenderDrawColor(renderer_, background_color_.r, background_color_.g, background_color_.b, background_color_.a);
  SDL_RenderClear(renderer_);

  load_text_ = new Text(renderer_, button_font_, button_text_color_, "Load File");
  load_button_ = new LabeledButton(renderer_, load_button_width_, load_button_height_, load_text_);
  load_button_event_ = NONE;

  begin_text_ = new Text(renderer_, button_font_, button_text_color_, "Begin");
  begin_button_ = new LabeledButton(renderer_, load_button_width_, load_button_height_, begin_text_);
  begin_button_event_ = NONE;

  return_button_text_ = new Text(renderer_, button_font_, button_text_color_, "Main Menu");
  return_button_ = new LabeledButton(renderer_, return_button_width_, return_button_height_, return_button_text_);
  return_button_event_ = NONE;

  explanation_text_ = new Text(renderer_,
                               small_font_,
                               small_font_color_,
                               "Use the buttons outside the canvas to choose and then load the file, then click Begin.",
                               1000);

}

void LoadSceneEmscripten::RunPostLoop() {

  delete load_text_;
  load_text_ = nullptr;
  delete load_button_;
  load_button_ = nullptr;
  load_button_event_ = NONE;

  delete begin_text_;
  begin_text_ = nullptr;
  delete begin_button_;
  begin_button_ = nullptr;
  begin_button_event_ = NONE;

  delete explanation_text_;
  explanation_text_ = nullptr;

  delete error_text_;
  error_text_ = nullptr;

  delete return_button_text_;
  return_button_text_ = nullptr;
  delete return_button_;
  return_button_ = nullptr;
  return_button_event_ = NONE;

  if (loaded_file_name_ != nullptr) {
    free(loaded_file_name_);
    loaded_file_name_ = nullptr;
  }

  delete word_loader_;
  word_loader_ = nullptr;

}

void LoadSceneEmscripten::ClearErrorMessage() {
  delete error_text_;
  error_text_ = nullptr;
}

void LoadSceneEmscripten::SetErrorMessage(std::string error_message) {
  ClearErrorMessage();
  error_text_ = new Text(renderer_, small_font_, small_font_color_, error_message, 1000);
}

void LoadSceneEmscripten::HandleBeginEvent(SDL_Event &event) {

  GameScene game_scene = GameScene(renderer_, window_, global_quit_, screen_height_, screen_width_,
                                   word_loader_->GetWordPairMap());
  game_scene.Run();

  // The loading scene should only be entered from the front; after the game scene runs, we actually want to exit
  // out of the load scene as well
  QuitLocal();

}

void LoadSceneEmscripten::RunSingleIterationEventHandler(SDL_Event &event) {

  if (event.type == SDL_QUIT) {
    QuitGlobal();
  }

  return_button_event_ = return_button_->HandleEvent(&event);
  load_button_event_ = load_button_->HandleEvent(&event);
  begin_button_event_ = begin_button_->HandleEvent(&event);

  if (return_button_event_ == PRESSED) {
    QuitLocal();
  }

  if (load_button_event_ == PRESSED) {
    loaded_file_has_been_processed_ = false;
    AllocateLoadedFileName();
    ClearErrorMessage();
#ifdef __EMSCRIPTEN__
    load_file(loaded_file_name_);
#endif
  }

  if (begin_button_event_ == PRESSED) {
    HandleBeginEvent(event);
  }

  if (IsFileLoaded() && !loaded_file_has_been_processed_) {

    ClearErrorMessage();

    printf("Processing file\n");
    word_loader_ = new FileWordLoader(kEmscriptenInputFilePath);
    WordLoader::InputError input_error = word_loader_->ParseAndLoadIntoMap();

    if (input_error != WordLoader::InputError::NONE) {
      switch (input_error) {
        case WordLoader::InputError::LINE_CONTAINS_MORE_THAN_ONE_COMMA:
          SetErrorMessage(
              "A line exists in the file with more than one comma; each line must have one comma. Please try again.");
          break;
        case WordLoader::InputError::LINE_CONTAINS_NO_COMMA:
          SetErrorMessage("A line exists in the file without a comma; each line must have one comma. Please try again.");
          break;
        case WordLoader::InputError::FILE_NOT_FOUND:
          SetErrorMessage("File not found. Please try again.");
          break;
        default:
          throw std::runtime_error(boost::str(boost::format("Unknown input error %1%") % input_error));
      }
    }

    printf("File was processed\n");
    loaded_file_has_been_processed_ = true;

  }

}

void LoadSceneEmscripten::AllocateLoadedFileName() {

  if (loaded_file_name_ != nullptr) {
    free(loaded_file_name_);
  }
  loaded_file_name_ = (char *) malloc(256);
  loaded_file_name_[0] = '\0';

}

bool LoadSceneEmscripten::IsFileLoaded() {
  return loaded_file_name_ != nullptr && strlen(loaded_file_name_) > 0;
}

bool LoadSceneEmscripten::IsFileReadyForGame() {
  return IsFileLoaded() && loaded_file_has_been_processed_ && error_text_ == nullptr;
}

void LoadSceneEmscripten::RunSingleIterationLoopBody() {

  if (IsFileLoaded()) {
    printf("Loaded file name: %s\n", loaded_file_name_);
  }

  SDL_SetRenderDrawColor(renderer_, background_color_.r, background_color_.g, background_color_.b, background_color_.a);
  SDL_RenderClear(renderer_);

  // Render load button in bottom middle
  load_button_->SetTopLeftPosition(screen_width_ / 2 - load_button_width_ / 2,
                                   screen_height_ - load_button_height_ - 300);
  load_button_->Render();

  // Render begin button below the load button
  if (IsFileReadyForGame()) {

    begin_button_->SetTopLeftPosition(screen_width_ / 2 - load_button_width_ / 2,
                                      screen_height_ - load_button_height_ - 150);
    begin_button_->Render();

  }


  // Render return button in bottom right
  return_button_->SetTopLeftPosition(screen_width_ - 10 - return_button_width_,
                                     screen_height_ - return_button_height_ - 10);
  return_button_->Render();

  // Render the explanation in the top middle
  explanation_text_->Render(screen_width_ / 2 - explanation_text_->GetWidth() / 2,
                            explanation_text_->GetHeight() + 100);

  if (error_text_ != nullptr) {
    error_text_->Render(screen_width_ / 2 - error_text_->GetWidth() / 2,
                        screen_height_ - load_button_height_ - 100);
  }

  SDL_RenderPresent(renderer_);

}

}