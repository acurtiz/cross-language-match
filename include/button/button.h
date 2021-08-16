#include <SDL2/SDL.h>
#include "shape/rectangle.h"
#include "button/button_event.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_BUTTON_H_
#define CROSSLANGUAGEMATCH_INCLUDE_BUTTON_H_

namespace cross_language_match {

class Button : public Rectangle {

 public:
  explicit Button(Rectangle rectangle);
  ButtonEvent HandleEvent(SDL_Event *event);
  void Render() override;

 private:
  const SDL_Color color_default_ = {0x93, 0xE9, 0xBE, 0xFF};
  const SDL_Color color_mouse_down_ = {0xDA, 0x70, 0xD6, 0xFF};
  const SDL_Color color_mouse_motion_ = {0xBB, 0xCC, 0xD6, 0xFF};

};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_BUTTON_H_
