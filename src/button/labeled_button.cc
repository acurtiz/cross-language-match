#include "button/labeled_button.h"

namespace cross_language_match {

LabeledButton::LabeledButton(SDL_Renderer *renderer, int width, int height, Text *label) :
    Button(renderer, width, height) {
  label_ = label;
}

void LabeledButton::Render() {

  int extra_width = Button::width_ - label_->GetWidth();
  int extra_height = Button::height_ - label_->GetHeight();

  if (extra_width < 0) {
    printf("Warning: button width (%d) is less than label width (%d), yet button needs to hold entire label\n",
           Button::width_,
           label_->GetWidth());
  }

  if (extra_height < 0) {
    printf("Warning: button height (%d) is less than label width (%d), yet button needs to hold entire label\n",
           Button::height_,
           label_->GetHeight());
  }

  int label_top_left_x = Button::top_left_x_ + extra_width / 2;
  int label_top_left_y = Button::top_left_y_ + extra_height / 2;

  Button::Render();
  label_->Render(label_top_left_x, label_top_left_y);
}

}