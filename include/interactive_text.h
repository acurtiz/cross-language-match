#include <SDL2/SDL.h>
#include <vector>
#include "text.h"

#ifndef CROSSLANGUAGEMATCH_SRC_INTERACTIVE_TEXT_H_
#define CROSSLANGUAGEMATCH_SRC_INTERACTIVE_TEXT_H_

namespace cross_language_match {

enum InteractiveTextGroup {
  LEFT = 0,
  RIGHT = 1
};

class InteractiveText {
 public:
  InteractiveText(SDL_Renderer *renderer, Text *text, InteractiveTextGroup group);
  ~InteractiveText();
  void AddHighlight();
  void RemoveHighlight();
  void AddLink(InteractiveText *other);
  void RemoveLink();
  InteractiveText *GetLink();
  void SetTopLeftPosition(int x, int y);
  void Render();
  void HandleEvent(SDL_Event *event, std::vector<InteractiveText *> all_text);
  Text *GetText();
  InteractiveTextGroup GetGroup();
  int GetWidth();
  int GetHeight();
  static int GetPaddingPerSide();
 private:
  static const int text_padding_per_side_ = 5;
  SDL_Renderer *renderer_;
  Text *text_;
  InteractiveText *linked_interactive_text_;
  bool is_highlighted_;
  int top_left_x_;
  int top_left_y_;
  int width_;
  int height_;
  InteractiveTextGroup group_;
  InteractiveText *GetHighlightedOtherFromSameGroup(std::vector<InteractiveText *> all_text);
  InteractiveText *GetHighlightedOtherFromDifferentGroup(std::vector<InteractiveText *> all_text);
};

}
#endif //CROSSLANGUAGEMATCH_SRC_INTERACTIVE_TEXT_H_
