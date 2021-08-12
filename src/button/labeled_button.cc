#include "button/labeled_button.h"

namespace cross_language_match {

LabeledButton::LabeledButton(Button button, Text *label) : Button(button) {
  label_ = label;
}

void LabeledButton::Render() {

  int extra_width = GetWidth() - label_->GetWidth();
  int extra_height = GetHeight() - label_->GetHeight();

  if (extra_width < 0) {
    printf("Warning: button width (%d) is less than label width (%d), yet button needs to hold entire label\n",
           GetWidth(),
           label_->GetWidth());
  }

  if (extra_height < 0) {
    printf("Warning: button height (%d) is less than label width (%d), yet button needs to hold entire label\n",
           GetHeight(),
           label_->GetHeight());
  }

  int label_top_left_x = GetTopLeftX() + extra_width / 2;
  int label_top_left_y = GetTopLeftY() + extra_height / 2;

  Button::Render();
  label_->Render(label_top_left_x, label_top_left_y);
}

}