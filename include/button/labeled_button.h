#include "rectangular_button.h"
#include "text/text.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_LABELED_BUTTON_H_
#define CROSSLANGUAGEMATCH_INCLUDE_LABELED_BUTTON_H_

namespace cross_language_match {

class LabeledButton : public RectangularButton {
 public:
  LabeledButton(RectangularButton button, Text *label);
  void Render() override;
 private:
  Text *label_;
};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_LABELED_BUTTON_H_
