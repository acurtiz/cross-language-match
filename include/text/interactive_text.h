#include <SDL2/SDL.h>
#include <vector>
#include "text.h"
#include "button/cancellation_circle_button.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_INTERACTIVE_TEXT_H_
#define CROSSLANGUAGEMATCH_INCLUDE_INTERACTIVE_TEXT_H_

namespace cross_language_match {

enum InteractiveTextGroup {
  LEFT = 0,
  RIGHT = 1
};

class InteractiveText : public Rectangle {

 public:
  InteractiveText(SDL_Renderer *renderer, Text *text, InteractiveTextGroup group);
  void AddHighlight();
  void RemoveHighlight();
  void AddLink(InteractiveText *other);
  void RemoveLink();
  InteractiveText *GetLink();
  void Render() override;
  void SetTopLeftPosition(int top_left_x, int top_left_y) override;
  void HandleEvent(SDL_Event *event, std::vector<InteractiveText *> all_text);
  Text *GetText();
  InteractiveTextGroup GetGroup();
  static int GetPaddingPerSide();

 private:

  static const int text_padding_per_side_ = 5;
  SDL_Renderer *renderer_;
  Text *text_;
  InteractiveText *linked_interactive_text_;
  bool is_highlighted_;
  InteractiveTextGroup group_;
  InteractiveText *GetHighlightedOtherFromSameGroup(std::vector<InteractiveText *> all_text);
  InteractiveText *GetHighlightedOtherFromDifferentGroup(std::vector<InteractiveText *> all_text);
  CancellationCircleButton *link_cancellation_circle_;
  int line_one_x1_;
  int line_one_y1_;
  int line_one_x2_;
  int line_one_y2_;
  int line_two_x1_;
  int line_two_y1_;
  int line_two_x2_;
  int line_two_y2_;

  SDL_Color interactive_line_color_;
  SDL_Color interactive_text_highlight_color_;
  SDL_Color interactive_text_non_highlight_color_;
};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_INTERACTIVE_TEXT_H_
