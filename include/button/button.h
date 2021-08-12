#include <SDL2/SDL.h>
#include "shape/rectangle.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_BUTTON_H_
#define CROSSLANGUAGEMATCH_INCLUDE_BUTTON_H_

namespace cross_language_match {

enum ButtonEvent {
  NONE = 0,
  PRESSED = 1
};

class Button : public Rectangle {
 public:
  Button(Rectangle rectangle);
  ButtonEvent HandleEvent(SDL_Event *event);
  virtual void Render();

 private:
  const SDL_Color color_default_ = {0x93, 0xE9, 0xBE, 0xFF};
  const SDL_Color color_mouse_down_ = {0xDA, 0x70, 0xD6, 0xFF};
  const SDL_Color color_mouse_motion_ = {0xBB, 0xCC, 0xD6, 0xFF};

};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_BUTTON_H_
