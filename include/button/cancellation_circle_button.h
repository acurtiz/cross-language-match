#include <SDL2/SDL.h>
#include "shape/circle.h"
#include "button/button.h"
#include "button/button_event.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_BUTTON_CANCELLATION_CIRCLE_BUTTON_H_
#define CROSSLANGUAGEMATCH_INCLUDE_BUTTON_CANCELLATION_CIRCLE_BUTTON_H_

namespace cross_language_match {

class CancellationCircleButton : public Circle, public Button {

 public:
  explicit CancellationCircleButton(SDL_Renderer *renderer);
  void Render() override;
  ButtonEvent HandleEvent(SDL_Event *event) override;
  void SetColor(SDL_Color color) override;

 private:
  SDL_Renderer *renderer_;
  SDL_Color color_;
  SDL_Color color_default_;
  SDL_Color color_mouse_down_;
  SDL_Color color_mouse_motion_;

};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_BUTTON_CANCELLATION_CIRCLE_BUTTON_H_
