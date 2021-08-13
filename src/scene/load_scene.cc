#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <boost/format.hpp>
#include "scene/load_scene.h"
#include "button/labeled_button.h"
#include "button/button.h"
#include "scene/game_scene.h"
#include "word_loader/file_word_loader.h"
#include <emscripten.h>

namespace cross_language_match {

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
    (const char *loaded_file_name),
    {
      var input = document.createElement('input');
      input.type = 'file';
      input.onchange = e => {

        var file_blob = e.target.files[0];

        if (file_blob.length == 0) {
          return;
        }

        var reader = new FileReader();

        reader.onload = function()
        {
          var data_str = reader.result;
          Module.ccall('persist_file', null,["string"],[data_str]);
          // Populate the passed in filename variable
          stringToUTF8(file_blob.name, loaded_file_name, lengthBytesUTF8(file_blob.name) + 1);
        };

        reader.readAsText(file_blob);

      };

      input.click();

    }
);

LoadScene::LoadScene(SDL_Renderer *renderer,
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

  button_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", wide_button_font_size_);
  if (button_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1%\n") % TTF_GetError()));
  }

  small_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", small_font_size_);
  if (small_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1%\n") % TTF_GetError()));
  }

}

LoadScene::~LoadScene() {

  TTF_CloseFont(button_font_);
  button_font_ = nullptr;

  TTF_CloseFont(small_font_);
  small_font_ = nullptr;

  TTF_Quit();

}

void LoadScene::RunPreLoop() {

  SDL_SetRenderDrawColor(renderer_, background_color_.r, background_color_.g, background_color_.b, background_color_.a);
  SDL_RenderClear(renderer_);

  load_text_ = new Text(renderer_, button_font_, button_text_color_, "Load File");
  load_button_ =
      new LabeledButton(Button(Rectangle(renderer_, wide_button_width_, wide_button_height_)), load_text_);
  load_button_event_ = NONE;

  begin_text_ = new Text(renderer_, button_font_, button_text_color_, "Begin");
  begin_button_ =
      new LabeledButton(Button(Rectangle(renderer_, wide_button_width_, wide_button_height_)), begin_text_);
  begin_button_event_ = NONE;

  return_button_text_ = new Text(renderer_, button_font_, button_text_color_, "Main Menu");
  return_button_ =
      new LabeledButton(Button(Rectangle(renderer_, return_button_width_, return_button_height_)), return_button_text_);
  return_button_event_ = NONE;

  explanation_text_ = new Text(renderer_,
                               small_font_,
                               small_font_color_,
                               "Use the buttons outside the canvas to choose and then load the file, then click Begin.",
                               1000);

  load_button_->SetTopLeftPosition(screen_width_ / 2 - load_button_->GetWidth() / 2,
                                   screen_height_ - load_button_->GetHeight() - 300);

  // Set begin button to be just below the load button
  begin_button_->SetTopLeftPosition(screen_width_ / 2 - begin_button_->GetWidth() / 2,
                                    load_button_->GetTopLeftY() + load_button_->GetHeight() + 10);

  // Set the return button to be in the bottom right
  return_button_->SetTopLeftPosition(screen_width_ - 10 - return_button_->GetWidth(),
                                     screen_height_ - return_button_->GetHeight() - 10);

  // Set the explanation to be in the top middle
  explanation_text_->SetTopLeftPosition(screen_width_ / 2 - explanation_text_->GetWidth() / 2,
                                        explanation_text_->GetHeight() + 100);

}

void LoadScene::RunPostLoop() {

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

void LoadScene::ClearErrorMessage() {
  delete error_text_;
  error_text_ = nullptr;
}

void LoadScene::SetErrorMessage(std::string error_message) {
  ClearErrorMessage();
  error_text_ = new Text(renderer_, small_font_, small_font_color_, error_message, 1000);
  error_text_->SetTopLeftPosition(screen_width_ / 2 - error_text_->GetWidth() / 2,
                                  screen_height_ - wide_button_height_ - 100);
}

void LoadScene::HandleBeginEvent(SDL_Event &event) {

  GameScene game_scene = GameScene(renderer_, window_, global_quit_, screen_height_, screen_width_,
                                   word_loader_->GetWordPairMap());
  game_scene.Run();

  // The loading scene should only be entered from the front; after the game scene runs, we actually want to exit
  // out of the load scene as well
  QuitLocal();

}

void LoadScene::RunSingleIterationEventHandler(SDL_Event &event) {

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
    load_file(loaded_file_name_);
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

    loaded_file_has_been_processed_ = true;

  }

}

void LoadScene::AllocateLoadedFileName() {

  if (loaded_file_name_ != nullptr) {
    free(loaded_file_name_);
  }
  loaded_file_name_ = (char *) malloc(256);
  loaded_file_name_[0] = '\0';

}

bool LoadScene::IsFileLoaded() {
  return loaded_file_name_ != nullptr && strlen(loaded_file_name_) > 0;
}

bool LoadScene::IsFileReadyForGame() {
  return IsFileLoaded() && loaded_file_has_been_processed_ && error_text_ == nullptr;
}

bool LoadScene::IsErrorMessageSet() {
  return error_text_ != nullptr;
}

void LoadScene::RunSingleIterationLoopBody() {

  SDL_SetRenderDrawColor(renderer_, background_color_.r, background_color_.g, background_color_.b, background_color_.a);
  SDL_RenderClear(renderer_);

  load_button_->Render();
  return_button_->Render();
  explanation_text_->Render();

  if (IsFileReadyForGame()) {
    begin_button_->Render();
  }

  if (IsErrorMessageSet()) {
    error_text_->Render();
  }

  SDL_RenderPresent(renderer_);

}

}