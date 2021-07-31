#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <boost/format.hpp>
#include "scene/load_scene.h"
#include "button/labeled_button.h"
#include "button/button.h"
#include "scene/game_scene.h"
#include "word_loader/file_word_loader.h"

namespace cross_language_match {

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

  button_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", load_button_font_size_);
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

  load_text_ = new Text(renderer_, button_font_, button_text_color_, "Load");
  load_button_ = new LabeledButton(renderer_, load_button_width_, load_button_height_, load_text_);
  load_button_event_ = NONE;

  return_button_text_ = new Text(renderer_, button_font_, button_text_color_, "Main Menu");
  return_button_ = new LabeledButton(renderer_, return_button_width_, return_button_height_, return_button_text_);
  return_button_event_ = NONE;

  explanation_text_ = new Text(renderer_,
                               small_font_,
                               small_font_color_,
                               "Enter CSV file path",
                               1000);

  input_text_ = new Text(renderer_, small_font_, small_font_color_, inputted_text_);

  SDL_StartTextInput();

}

void LoadScene::RunPostLoop() {

  delete load_text_;
  load_text_ = nullptr;

  delete load_button_;
  load_button_ = nullptr;

  load_button_event_ = NONE;

  delete explanation_text_;
  explanation_text_ = nullptr;

  delete error_text_;
  error_text_ = nullptr;

  delete return_button_text_;
  return_button_text_ = nullptr;

  delete return_button_;
  return_button_ = nullptr;

  SDL_StopTextInput();

}

void LoadScene::RunSingleIterationEventHandler(SDL_Event &event) {

  if (event.type == SDL_QUIT) {
    QuitGlobal();
  }

  return_button_event_ = return_button_->HandleEvent(&event);
  load_button_event_ = load_button_->HandleEvent(&event);

  if (event.type == SDL_KEYDOWN) {

    if (event.key.keysym.sym == SDLK_BACKSPACE && inputted_text_.length() > 0) {
      inputted_text_.pop_back();
      reload_input_text_ = true;
      printf("Deleting character; string is now: %s\n", inputted_text_.c_str());
    } else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
      SDL_SetClipboardText(inputted_text_.c_str());
      printf("Copied string to clipboard");
    } else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
      inputted_text_ = SDL_GetClipboardText();
      reload_input_text_ = true;
      printf("Pasting string, string is now: %s\n", inputted_text_.c_str());
    } else if (event.key.keysym.sym == SDLK_RETURN) {
      inputted_text_.push_back('\n');
    }

  } else if (event.type == SDL_TEXTINPUT) {

    bool is_control_pressed = SDL_GetModState() & KMOD_CTRL;
    bool is_c_or_v_pressed = event.text.text[0] == 'C' || event.text.text[0] == 'c' || event.text.text[0] == 'V'
        || event.text.text[0] == 'v';

    if (!(is_control_pressed && is_c_or_v_pressed)) {
      inputted_text_ += event.text.text;
      reload_input_text_ = true;
      printf("Adding character to string, string is now: %s\n", inputted_text_.c_str());

    }
  }

  if (reload_input_text_) {
    delete input_text_;
    // SDL_TTF cannot handle empty string
    input_text_ = new Text(renderer_, small_font_, small_font_color_,
                           inputted_text_.empty() ? " " : inputted_text_);
    reload_input_text_ = false;
  }

  if (return_button_event_ == PRESSED) {
    QuitLocal();
  }

  if (load_button_event_ == PRESSED) {

    FileWordLoader word_loader = FileWordLoader(inputted_text_);
    WordLoader::InputError input_error = word_loader.ParseAndLoadIntoMap();

    if (input_error == WordLoader::InputError::NONE) {

      delete error_text_;
      error_text_ = nullptr;

    } else {

      std::string error_message;
      switch (input_error) {
        case WordLoader::InputError::LINE_CONTAINS_MORE_THAN_ONE_COMMA:
          error_message =
              "A line exists in the file with more than one comma; each line must have one comma. Please try again.";
          break;
        case WordLoader::InputError::LINE_CONTAINS_NO_COMMA:
          error_message =
              "A line exists in the file without a comma; each line must have one comma. Please try again.";
          break;
        case WordLoader::InputError::FILE_NOT_FOUND:
          error_message =
              "File not found. Please try again.";
          break;
        default:
          throw std::runtime_error(boost::str(boost::format("Unknown input error %1%") % input_error));
      }
      error_text_ = new Text(renderer_, small_font_, small_font_color_, error_message, 1000);

    }

    if (error_text_ == nullptr) {

      GameScene game_scene = GameScene(renderer_, window_, global_quit_, screen_height_, screen_width_,
                                       word_loader.GetWordPairMap());
      game_scene.Run();

      // The loading scene should only be entered from the front; after the game scene runs, we actually want to exit
      // out of the load scene as well
      QuitLocal();

    }

  }

}

void LoadScene::RunSingleIterationLoopBody() {

  SDL_SetRenderDrawColor(renderer_, background_color_.r, background_color_.g, background_color_.b, background_color_.a);
  SDL_RenderClear(renderer_);

  // Render load button in bottom middle
  load_button_->SetTopLeftPosition(screen_width_ / 2 - load_button_width_ / 2,
                                   screen_height_ - load_button_height_ - 300);
  load_button_->Render();

  // Render return button in bottom right
  return_button_->SetTopLeftPosition(screen_width_ - 10 - return_button_width_,
                                     screen_height_ - return_button_height_ - 10);
  return_button_->Render();

  // Render the explanation in the top middle
  explanation_text_->Render(screen_width_ / 2 - explanation_text_->GetWidth() / 2,
                            explanation_text_->GetHeight() + 100);

  // Render the input text
  input_text_->Render(screen_width_ / 2 - input_text_->GetWidth() / 2,
                      explanation_text_->GetHeight() + 100 + input_text_->GetHeight() + 100);

  if (error_text_ != nullptr) {
    error_text_->Render(screen_width_ / 2 - error_text_->GetWidth() / 2,
                        screen_height_ - load_button_height_ - 100);
  }

  SDL_RenderPresent(renderer_);

}

}