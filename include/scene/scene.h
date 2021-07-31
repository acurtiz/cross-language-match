#include <SDL2/SDL.h>

#ifndef CROSSLANGUAGEMATCH_SRC_SCENE_H_
#define CROSSLANGUAGEMATCH_SRC_SCENE_H_

namespace cross_language_match {

class Scene {

 public:
  Scene(SDL_Renderer *renderer, SDL_Window *window, bool &global_quit);
  ~Scene();
  void Run();

 protected:

  virtual void RunPreLoop() = 0;
  virtual void RunPostLoop() = 0;
  virtual void RunSingleIterationEventHandler(SDL_Event &event) = 0;
  virtual void RunSingleIterationLoopBody() = 0;
  void QuitLocal();
  void QuitGlobal();

  SDL_Renderer *renderer_;
  SDL_Window *window_;
  bool &global_quit_;
  bool local_quit_;

  SDL_Color background_color_ = {0xFF, 0x7F, 0x50, 0xFF};

};

}
#endif //CROSSLANGUAGEMATCH_SRC_SCENE_H_
