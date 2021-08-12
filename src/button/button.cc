#include "button/button.h"

namespace cross_language_match {

Button::Button(Rectangle rectangle) : Rectangle(rectangle) {
  SetColor(color_default_);
}

ButtonEvent Button::HandleEvent(SDL_Event *event) {

  if (!(event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)) {
    return NONE;
  }

  SetColor(color_default_);

  if (IsMouseInside()) {

    switch (event->type) {
      case SDL_MOUSEBUTTONDOWN:
        SetColor(color_mouse_down_);
        break;
      case SDL_MOUSEMOTION:
        SetColor(color_mouse_motion_);
        break;
      case SDL_MOUSEBUTTONUP:
        return PRESSED;
      default:
        printf("Unrecognized mouse event type: %d", event->type);
        return NONE;
    }
  }

  return NONE;

}

void Button::Render() {
  Rectangle::Render();
}

}