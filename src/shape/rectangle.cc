#include <SDL2/SDL.h>
#include "shape/rectangle.h"

namespace cross_language_match {

Rectangle::Rectangle(SDL_Renderer *renderer) {
  renderer_ = renderer;
}

Rectangle::Rectangle(SDL_Renderer *renderer, int width, int height) {
  renderer_ = renderer;
  width_ = width;
  height_ = height;
}

void Rectangle::SetTopLeftPosition(int top_left_x, int top_left_y) {
  top_left_x_ = top_left_x;
  top_left_y_ = top_left_y;
}

void Rectangle::SetColor(SDL_Color color) {
  color_ = color;
}

void Rectangle::SetHeight(int height) {
  height_ = height;
}

void Rectangle::SetWidth(int width) {
  width_ = width;
}

int Rectangle::GetHeight() {
  return height_;
}

int Rectangle::GetWidth() {
  return width_;
}

int Rectangle::GetTopLeftX() {
  return top_left_x_;
}

int Rectangle::GetTopLeftY() {
  return top_left_y_;
}

SDL_Color Rectangle::GetColor() {
  return color_;
}

void Rectangle::Render() {
  SDL_Rect rect = {top_left_x_, top_left_y_, width_, height_};
  SDL_SetRenderDrawColor(renderer_, color_.r, color_.g, color_.b, color_.a);
  SDL_RenderFillRect(renderer_, &rect);
}

bool Rectangle::IsMouseInside() {

  int mouse_x, mouse_y = 0;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  return (mouse_x >= GetTopLeftX())
      && (mouse_x <= GetTopLeftX() + GetWidth())
      && (mouse_y >= GetTopLeftY())
      && (mouse_y <= GetTopLeftY() + GetHeight());

}

}