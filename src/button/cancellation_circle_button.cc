#include <cmath>
#include "button/cancellation_circle_button.h"

namespace cross_language_match {

CancellationCircleButton::CancellationCircleButton(SDL_Renderer *renderer)
    : Circle(renderer),
      renderer_(renderer),
      color_({0x48, 0x3C, 0x32, 0xFF}),
      color_default_({0x48, 0x3C, 0x32, 0xFF}),
      color_mouse_down_({0x1A, 0x56, 0x53, 0xFF}),
      color_mouse_motion_({0x4E, 0xC3, 0x3D, 0xFF}) {}

void CancellationCircleButton::Render() {

  Circle::Render();

  int side = (int) sqrt(pow(GetRadius(), 2) / 2);

  int top_left_x = GetCenterX() - side;
  int top_left_y = GetCenterY() + side;

  int bottom_right_x = GetCenterX() + side;
  int bottom_right_y = GetCenterY() - side;

  int top_right_x = GetCenterX() + side;
  int top_right_y = GetCenterY() + side;

  int bottom_left_x = GetCenterX() - side;
  int bottom_left_y = GetCenterY() - side;

  Uint8 already_set_r, already_set_g, already_set_b, already_set_a = 0;

  SDL_GetRenderDrawColor(renderer_, &already_set_r, &already_set_g, &already_set_b, &already_set_a);

  SDL_SetRenderDrawColor(renderer_, color_.r, color_.g, color_.b, color_.a);
  SDL_RenderDrawLine(renderer_, top_left_x, top_left_y, bottom_right_x, bottom_right_y);
  SDL_RenderDrawLine(renderer_, bottom_left_x, bottom_left_y, top_right_x, top_right_y);

  SDL_SetRenderDrawColor(renderer_, already_set_r, already_set_g, already_set_b, already_set_a);

}

void CancellationCircleButton::SetColor(SDL_Color color) {

  Circle::SetColor(color);
  color_ = color;

}

ButtonEvent CancellationCircleButton::HandleEvent(SDL_Event *event) {

  if (!(event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEMOTION)) {
    return NONE;
  }

  SetColor(color_default_);

  if (IsMouseInside()) {

    switch (event->type) {
      case SDL_MOUSEBUTTONUP:
        return PRESSED;
      case SDL_MOUSEMOTION:
        SetColor(color_mouse_motion_);
        break;
      case SDL_MOUSEBUTTONDOWN:
        SetColor(color_mouse_down_);
        break;
      default:
        printf("Unrecognized mouse event type: %d", event->type);
        return NONE;
    }
  }

  return NONE;

}

}