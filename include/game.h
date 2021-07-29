#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
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
  const double screen_width_ = 1280;
  const double screen_height_ = 720;
  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
  bool global_quit_ = false;

};

}
#endif //CROSSLANGUAGEMATCH_SRC_GAME_H_
