#include "button/button_event.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_BUTTON_BUTTON_H_
#define CROSSLANGUAGEMATCH_INCLUDE_BUTTON_BUTTON_H_

namespace cross_language_match {

class Button {

 public:
  virtual ButtonEvent HandleEvent(SDL_Event *event) = 0;

};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_BUTTON_BUTTON_H_
