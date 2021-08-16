#ifndef CROSSLANGUAGEMATCH_INCLUDE_SHAPE_CIRCLE_H_
#define CROSSLANGUAGEMATCH_INCLUDE_SHAPE_CIRCLE_H_

namespace cross_language_match {

class Circle {

 public:
  explicit Circle(SDL_Renderer *renderer);
  void SetRadius(int radius);
  void SetCenter(int x, int y);
  void SetColor(SDL_Color color);
  virtual void Render();
  bool IsMouseInside();
  int GetRadius();
  int GetCenterX();
  int GetCenterY();

 private:
  SDL_Renderer *renderer_;
  SDL_Color color_;
  int radius_;
  int center_x_;
  int center_y_;

};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_SHAPE_CIRCLE_H_
