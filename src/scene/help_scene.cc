#include <boost/format.hpp>
#include <button/labeled_button.h>
#include "scene/help_scene.h"

namespace cross_language_match {

HelpScene::HelpScene(SDL_Renderer *renderer,
                     SDL_Window *window,
                     bool &global_quit,
                     int screen_height,
                     int screen_width)
    : Scene(renderer, window, global_quit),
      screen_height_(screen_height),
      screen_width_(screen_width) {

  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

  if (TTF_Init() == -1) {
    throw std::runtime_error(
        boost::str(boost::format("SDL_ttf could not be initialized, error: %1\n") % TTF_GetError())
    );
  }

  return_button_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", return_button_font_size_);
  if (return_button_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1\n") % TTF_GetError()));
  }

  explanation_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", explanation_font_size_);
  if (explanation_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1\n") % TTF_GetError()));
  }

}

HelpScene::~HelpScene() {

  TTF_CloseFont(return_button_font_);
  return_button_font_ = nullptr;

  TTF_CloseFont(explanation_font_);
  explanation_font_ = nullptr;

  TTF_Quit();

}

void HelpScene::RunPreLoop() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  return_text_ = new Text(renderer_, return_button_font_, return_text_color_, "Return to Main Menu");
  return_button_ = new LabeledButton(renderer_, return_button_width_, return_button_height_, return_text_);
  return_button_event_ = NONE;

  explanation_text_ = new Text(renderer_,
                               explanation_font_,
                               explanation_text_color_,
                               "In this game, your goal is to "
                               "match each word or phrase on the left column "
                               "with the corresponding word or phrase in the right column. "
                               "Left-click to select a word, and left click on the corresponding word to form a link. "
                               "Right-click an existing word that is linked to remove that link.", 1000);

}

void HelpScene::RunPostLoop() {

  delete return_text_;
  return_text_ = nullptr;

  return_button_event_ = NONE;

  delete explanation_text_;
  explanation_text_ = nullptr;

}

void HelpScene::RunSingleIterationEventHandler(SDL_Event &event) {

  if (event.type == SDL_QUIT) {
    QuitGlobal();
  }

  return_button_event_ = return_button_->HandleEvent(&event);

  if (return_button_event_ == PRESSED) {
    QuitLocal();
  }

}

void HelpScene::RunSingleIterationLoopBody() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);

  // Render submit button in bottom middle
  return_button_->SetTopLeftPosition(screen_width_ / 2 - return_button_width_ / 2,
                                     screen_height_ - return_button_height_ - 100);
  return_button_->Render();

  // Render the game title in the top middle
  explanation_text_->Render(screen_width_ / 2 - explanation_text_->GetWidth() / 2,
                            explanation_text_->GetHeight() + 100);

  SDL_RenderPresent(renderer_);

}

}