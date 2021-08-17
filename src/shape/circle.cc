#include <SDL2/SDL.h>
#include "shape/circle.h"

namespace cross_language_match {

Circle::Circle(SDL_Renderer *renderer) : color_({0, 0, 0, 0xFF}),
                                         renderer_(renderer),
                                         center_x_(0),
                                         center_y_(0),
                                         radius_(0) {}

void Circle::Render() {

  Uint8 already_set_r, already_set_g, already_set_b, already_set_a = 0;
  SDL_GetRenderDrawColor(renderer_, &already_set_r, &already_set_g, &already_set_b, &already_set_a);

  SDL_SetRenderDrawColor(renderer_, color_.r, color_.g, color_.b, color_.a);

  const int diameter = (radius_ * 2);

  int x = (radius_ - 1);
  int y = 0;
  int tx = 1;
  int ty = 1;
  int error = (tx - diameter);

  while (x >= y) {

    //  Each of the following renders an octant of the circle
    SDL_RenderDrawPoint(renderer_, center_x_ + x, center_y_ - y);
    SDL_RenderDrawPoint(renderer_, center_x_ + x, center_y_ + y);
    SDL_RenderDrawPoint(renderer_, center_x_ - x, center_y_ - y);
    SDL_RenderDrawPoint(renderer_, center_x_ - x, center_y_ + y);
    SDL_RenderDrawPoint(renderer_, center_x_ + y, center_y_ - x);
    SDL_RenderDrawPoint(renderer_, center_x_ + y, center_y_ + x);
    SDL_RenderDrawPoint(renderer_, center_x_ - y, center_y_ - x);
    SDL_RenderDrawPoint(renderer_, center_x_ - y, center_y_ + x);

    if (error <= 0) {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0) {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }

  SDL_SetRenderDrawColor(renderer_, already_set_r, already_set_g, already_set_b, already_set_a);

}

void Circle::SetCenter(int x, int y) {
  center_x_ = x;
  center_y_ = y;
}

void Circle::SetRadius(int radius) {
  radius_ = radius;
}

void Circle::SetColor(SDL_Color color) {
  color_ = color;
}

bool Circle::IsMouseInside() {

  int mouse_x, mouse_y = 0;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  double distance_squared =
      (mouse_x - center_x_) * (mouse_x - center_x_) + (mouse_y - center_y_) * (mouse_y - center_y_);

  double radius_squared = radius_ * radius_;

  return distance_squared < radius_squared;

}

int Circle::GetRadius() {
  return radius_;
}

int Circle::GetCenterX() {
  return center_x_;
}

int Circle::GetCenterY() {
  return center_y_;
}

}
