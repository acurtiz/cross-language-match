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
  void Render(int x, int y);
  Text *GetText();
 private:
  SDL_Renderer *renderer_;
  Text *text_;
  InteractiveText *linked_interactive_text_;
  bool is_highlighted_;
};

}
#endif //CROSSLANGUAGEMATCH_SRC_INTERACTIVE_TEXT_H_
