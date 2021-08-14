#include <SDL2/SDL.h>
#include <vector>
#include "text.h"

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

  SDL_Color interactive_line_color_ = {0x48, 0x3C, 0x32, 0xFF};
  SDL_Color interactive_text_highlight_color_ = {0x4E, 0xC3, 0x3D, 0xFF};
  SDL_Color interactive_text_non_highlight_color_ = {0x48, 0x3C, 0x32, 0xFF};
};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_INTERACTIVE_TEXT_H_
