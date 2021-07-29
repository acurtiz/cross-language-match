#include "button.h"
#include "text.h"

#ifndef CROSSLANGUAGEMATCH_SRC_LABELED_BUTTON_H_
#define CROSSLANGUAGEMATCH_SRC_LABELED_BUTTON_H_

namespace cross_language_match {

class LabeledButton : public Button {
 public:
  LabeledButton(SDL_Renderer *renderer, int width, int height, Text *label);
  void Render() override;
 private:
  Text *label_;
};

}

#endif //CROSSLANGUAGEMATCH_SRC_LABELED_BUTTON_H_
