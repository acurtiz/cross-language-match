#include "button.h"
#include "text/text.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_LABELED_BUTTON_H_
#define CROSSLANGUAGEMATCH_INCLUDE_LABELED_BUTTON_H_

namespace cross_language_match {

class LabeledButton : public Button {
 public:
  LabeledButton(Button button, Text *label);
  void Render();
 private:
  Text *label_;
};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_LABELED_BUTTON_H_
