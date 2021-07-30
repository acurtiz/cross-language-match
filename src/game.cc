#include <SDL2/SDL.h>
#include <boost/format.hpp>
#include <scene/start_scene.h>
#include "game.h"
#include "scene/game_scene.h"

namespace cross_language_match {

Game::Game() {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error(
        boost::str(boost::format("SDL could not initialize, error: %1%\n") % SDL_GetError())
    );
  }

  window_ = SDL_CreateWindow("Cross Language Match",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             screen_width_,
                             screen_height_,
                             SDL_WINDOW_SHOWN);

  if (window_ == nullptr) {
    throw std::runtime_error(
        boost::str(boost::format("Window could not be created, error: %1%\n") % SDL_GetError())
    );
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

  if (renderer_ == nullptr) {
    throw std::runtime_error(
        boost::str(boost::format("Renderer could not be created, error: %1%\n") % SDL_GetError())
    );
  }

}

Game::~Game() {

  SDL_DestroyWindow(window_);
  window_ = nullptr;
  SDL_Quit();

}

void Game::Run() {

  StartScene start_scene = StartScene(renderer_, window_, global_quit_,
                                      (int) screen_height_, (int) screen_width_);
  start_scene.Run();

}

}