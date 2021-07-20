//
// Created by Alex Curtis on 7/20/21.
//

#include <SDL2/SDL.h>
#include <text.h>

#ifndef CROSSLANGUAGEMATCH_SRC_INTERACTIVE_TEXT_H_
#define CROSSLANGUAGEMATCH_SRC_INTERACTIVE_TEXT_H_

namespace cross_language_match {

class InteractiveText {
 public:
  InteractiveText(SDL_Renderer *renderer, Text *text);
  ~InteractiveText();
  void AddHighlight();
  void RemoveHighlight();
  void AddLink(InteractiveText *other);
  void RemoveLink();
  InteractiveText *GetLink();
  void SetTopLeftPosition(int x, int y);
  void Render();
  void HandleEvent(SDL_Event *event);
  Text *GetText();
 private:
  int text_padding_per_side_ = 5;
  SDL_Renderer *renderer_;
  Text *text_;
  InteractiveText *linked_interactive_text_;
  bool is_highlighted_;
  int top_left_x_;
  int top_left_y_;
  int width_;
  int height_;
};

}
#endif //CROSSLANGUAGEMATCH_SRC_INTERACTIVE_TEXT_H_
