#include <boost/format.hpp>
#include "button/labeled_button.h"
#include "scene/load_scene.h"
#include "scene/start_scene.h"
#include "scene/game_scene.h"
#include "scene/help_scene.h"

namespace cross_language_match {

StartScene::StartScene(SDL_Renderer *renderer,
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

  button_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", button_font_size_);
  if (button_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1%\n") % TTF_GetError()));
  }

  title_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", title_font_size_);
  if (title_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1%\n") % TTF_GetError()));
  }

}

StartScene::~StartScene() {

  TTF_CloseFont(button_font_);
  button_font_ = nullptr;

  TTF_CloseFont(title_font_);
  title_font_ = nullptr;

  TTF_Quit();

}

void StartScene::RunPreLoop() {

  SDL_SetRenderDrawColor(renderer_, background_color_.r, background_color_.g, background_color_.b, background_color_.a);
  SDL_RenderClear(renderer_);

  start_text_ = new Text(renderer_, button_font_, start_text_color_, "Start the game");
  start_button_ =
      new LabeledButton(renderer_, start_button_width_, start_button_height_, start_text_);
  start_button_event_ = NONE;

  help_text_ = new Text(renderer_, button_font_, help_text_color_, "How to play");
  help_button_ = new LabeledButton(renderer_, help_button_width_, help_button_height_, help_text_);
  help_button_event_ = NONE;

  title_text_ = new Text(renderer_, title_font_, title_text_color_, "Cross Language Match");

}

void StartScene::RunPostLoop() {

  delete start_text_;
  start_text_ = nullptr;

  delete start_button_;
  start_button_ = nullptr;

  delete help_text_;
  help_text_ = nullptr;

  delete help_button_;
  help_button_ = nullptr;

  delete title_text_;
  title_text_ = nullptr;

  start_button_event_ = NONE;
  help_button_event_ = NONE;

}

void StartScene::RunSingleIterationEventHandler(SDL_Event &event) {

  if (event.type == SDL_QUIT) {
    QuitGlobal();
  }

  help_button_event_ = help_button_->HandleEvent(&event);
  start_button_event_ = start_button_->HandleEvent(&event);

  if (start_button_event_ == PRESSED) {

    LoadScene
        load_scene = LoadScene(renderer_, window_, global_quit_, screen_height_, screen_width_);
    load_scene.Run();

  } else if (help_button_event_ == PRESSED) {

    printf("Help button pressed. Going into help menu\n");
    HelpScene help_scene = HelpScene(renderer_, window_, global_quit_, screen_height_, screen_width_);
    help_scene.Run();

  }

}

void StartScene::RunSingleIterationLoopBody() {

  SDL_SetRenderDrawColor(renderer_, background_color_.r, background_color_.g, background_color_.b, background_color_.a);
  SDL_RenderClear(renderer_);

  // Render start button in bottom middle
  start_button_->SetTopLeftPosition(screen_width_ / 2 - start_button_width_ / 2,
                                    screen_height_ - start_button_height_ - 100);
  start_button_->Render();

  // Render the help button above the start button
  help_button_->SetTopLeftPosition(screen_width_ / 2 - help_button_width_ / 2,
                                   screen_height_ - help_button_height_ - start_button_height_ - 200);
  help_button_->Render();

  // Render the game title in the top middle
  title_text_->Render(screen_width_ / 2 - title_text_->GetWidth() / 2,
                      title_text_->GetHeight() + 100);

  SDL_RenderPresent(renderer_);

}

}