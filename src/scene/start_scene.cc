#include <boost/format.hpp>
#include <button/labeled_button.h>
#include "scene/start_scene.h"
#include "scene/game_scene.h"

namespace cross_language_match {

StartScene::StartScene(SDL_Renderer *renderer,
                       SDL_Window *window,
                       bool &global_quit,
                       int screen_height,
                       int screen_width)
    : Scene(renderer, window, global_quit),
      screen_height_((double) screen_height),
      screen_width_((double) screen_width) {

  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

  if (TTF_Init() == -1) {
    throw std::runtime_error(
        boost::str(boost::format("SDL_ttf could not be initialized, error: %1\n") % TTF_GetError())
    );
  }

  start_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", start_font_size_);
  if (start_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1\n") % TTF_GetError()));
  }

  title_font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", title_font_size_);
  if (title_font_ == nullptr) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1\n") % TTF_GetError()));
  }

}

StartScene::~StartScene() {

  TTF_CloseFont(start_font_);
  start_font_ = nullptr;

  TTF_CloseFont(title_font_);
  title_font_ = nullptr;

  TTF_Quit();

}

void StartScene::RunPreLoop() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  start_text_ = new Text(renderer_, start_font_, start_text_color_, "Start the game!");
  start_button_ = new LabeledButton(renderer_, start_button_width_, start_button_height_, start_text_);
  start_button_event_ = NONE;

  title_text_ = new Text(renderer_, title_font_, title_text_color_, "Cross Language Match");

}

void StartScene::RunPostLoop() {

  delete start_text_;
  start_text_ = nullptr;

  start_button_event_ = NONE;

  delete title_text_;
  title_text_ = nullptr;

}

void StartScene::RunSingleIterationEventHandler(SDL_Event &event) {

  if (event.type == SDL_QUIT) {
    QuitGlobal();
  }

  start_button_event_ = start_button_->HandleEvent(&event);

  if (start_button_event_ == PRESSED) {
    printf("Start button pressed. Going into main game\n");
    GameScene game_scene = GameScene(renderer_, window_, global_quit_, screen_height_, screen_width_);
    game_scene.LoadWordsViaFile("assets/txt/test-pairs.csv");
    game_scene.Run();
  }

}

void StartScene::RunSingleIterationLoopBody() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);

  // Render submit button in bottom middle
  start_button_->SetTopLeftPosition(screen_width_ / 2 - start_button_width_ / 2,
                                    screen_height_ - start_button_height_ - 100);
  start_button_->Render();

  // Render the game title in the top middle
  title_text_->Render(screen_width_ / 2 - title_text_->GetWidth() / 2,
                      title_text_->GetHeight() + 100);

  SDL_RenderPresent(renderer_);

}

}