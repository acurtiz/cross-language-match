#include <SDL2/SDL.h>
#include "shape/circle.h"
#include "button/button_event.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_SHAPE_CIRCLE_WITH_X_H_
#define CROSSLANGUAGEMATCH_INCLUDE_SHAPE_CIRCLE_WITH_X_H_

namespace cross_language_match {

class CircleWithX : public Circle {

 public:
  explicit CircleWithX(SDL_Renderer *renderer);
  void Render() override;
  ButtonEvent HandleEvent(SDL_Event *event);

 private:
  SDL_Renderer *renderer_;

};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_SHAPE_CIRCLE_WITH_X_H_
