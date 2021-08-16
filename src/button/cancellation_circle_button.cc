#include <cmath>
#include "button/cancellation_circle_button.h"

namespace cross_language_match {

CancellationCircleButton::CancellationCircleButton(SDL_Renderer *renderer)
    : Circle(renderer), renderer_(renderer) {}

void CancellationCircleButton::Render() {

  Circle::Render();

  // We only render the X inside if a mouse is in the circle
  if (!IsMouseInside()) {
    return;
  }

  int a = (int) sqrt(pow(GetRadius(), 2) / 2);

  int top_left_x = GetCenterX() - a;
  int top_left_y = GetCenterY() + a;

  int bottom_right_x = GetCenterX() + a;
  int bottom_right_y = GetCenterY() - a;

  int top_right_x = GetCenterX() + a;
  int top_right_y = GetCenterY() + a;

  int bottom_left_x = GetCenterX() - a;
  int bottom_left_y = GetCenterY() - a;

  SDL_RenderDrawLine(renderer_, top_left_x, top_left_y, bottom_right_x, bottom_right_y);
  SDL_RenderDrawLine(renderer_, bottom_left_x, bottom_left_y, top_right_x, top_right_y);

}

ButtonEvent CancellationCircleButton::HandleEvent(SDL_Event *event) {

  if (event->type != SDL_MOUSEBUTTONUP) {
    return NONE;
  }

  if (IsMouseInside()) {

    switch (event->type) {
      case SDL_MOUSEBUTTONUP:
        printf("RectangularButton up detected\n");
        return PRESSED;
      default:
        printf("Unrecognized mouse event type: %d", event->type);
        return NONE;
    }
  }

  return NONE;

}

}