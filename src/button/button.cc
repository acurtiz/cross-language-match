#include "button/button.h"

namespace cross_language_match {

Button::Button(SDL_Renderer *renderer, int width, int height) {
  renderer_ = renderer;
  width_ = width;
  height_ = height;
}

Button::~Button() {
  return;
}

ButtonEvent Button::HandleEvent(SDL_Event *event) {

  if (!(event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP)) {
    return NONE;
  }

  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  bool mouse_inside_button = (mouse_x >= top_left_x_)
      && (mouse_x <= top_left_x_ + width_)
      && (mouse_y >= top_left_y_)
      && (mouse_y <= top_left_y_ + height_);

  current_color_ = initial_color_;

  if (mouse_inside_button) {

    switch (event->type) {
      case SDL_MOUSEBUTTONDOWN:
        current_color_ = {0xDA, 0x70, 0xD6, 0xFF};
        return PRESSED;
      case SDL_MOUSEMOTION:
        current_color_ = {0xBB, 0xCC, 0xD6, 0xFF};
        break;
      case SDL_MOUSEBUTTONUP:
        current_color_ = initial_color_;
        break;
      default:
        printf("Unrecognized mouse event type: %d", event->type);
        return NONE;
    }
  }
}

void Button::Render() {

  SDL_Rect button_rect = {top_left_x_, top_left_y_, width_, height_};
  SDL_SetRenderDrawColor(renderer_, current_color_.r, current_color_.g, current_color_.b, current_color_.a);
  SDL_RenderFillRect(renderer_, &button_rect);

}

void Button::SetTopLeftPosition(int x, int y) {
  top_left_x_ = x;
  top_left_y_ = y;
}

}