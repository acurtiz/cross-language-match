#include <SDL2/SDL.h>

#ifndef CROSSLANGUAGEMATCH_INCLUDE_SHAPE_RECTANGLE_H_
#define CROSSLANGUAGEMATCH_INCLUDE_SHAPE_RECTANGLE_H_

namespace cross_language_match {

class Rectangle {

 public:
  Rectangle(SDL_Renderer *renderer, int width, int height);
  void SetWidth(int width);
  void SetHeight(int height);
  void SetTopLeftPosition(int top_left_x, int top_left_y);
  void SetColor(SDL_Color color);
  int GetHeight();
  int GetWidth();
  int GetTopLeftX();
  int GetTopLeftY();
  bool IsMouseInside();
  SDL_Color GetColor();
  virtual void Render();

 private:
  int width_ = 0;
  int height_ = 0;
  int top_left_x_ = 0;
  int top_left_y_ = 0;
  SDL_Color color_ = {0, 0, 0, 0xFF};
  SDL_Renderer *renderer_;

};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_SHAPE_RECTANGLE_H_
