#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <map>
#include "text/interactive_text.h"

#ifndef CROSSLANGUAGEMATCH_SRC_GAME_H_
#define CROSSLANGUAGEMATCH_SRC_GAME_H_

namespace cross_language_match {

class Game {

 public:
  Game();
  ~Game();
  void Run();

 private:
  const int screen_width_ = 1280;
  const int screen_height_ = 720;
  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
  bool global_quit_ = false;

};

}
#endif //CROSSLANGUAGEMATCH_SRC_GAME_H_
