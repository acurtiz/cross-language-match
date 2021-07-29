#ifndef CROSSLANGUAGEMATCH_SRC_BUTTON_H_
#define CROSSLANGUAGEMATCH_SRC_BUTTON_H_

#include <SDL2/SDL.h>

namespace cross_language_match {

enum ButtonEvent {
  NONE = 0,
  PRESSED = 1
};

class Button {
 public:
  Button(SDL_Renderer *renderer, int width, int height);
  ~Button();
  ButtonEvent HandleEvent(SDL_Event *event);
  void SetTopLeftPosition(int x, int y);
  virtual void Render();
 protected:
  int top_left_x_;
  int top_left_y_;
  int width_;
  int height_;
 private:
  SDL_Renderer *renderer_;
  const SDL_Color initial_color_ = {0xAA, 0xAA, 0xD6, 0xFF};
  SDL_Color current_color_ = initial_color_;

};

}

#endif //CROSSLANGUAGEMATCH_SRC_BUTTON_H_
