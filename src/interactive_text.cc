//
// Created by Alex Curtis on 7/20/21.
//

#include "interactive_text.h"

namespace cross_language_match {

InteractiveText::InteractiveText(SDL_Renderer *renderer, Text *text) {
  renderer_ = renderer;
  text_ = text;
  linked_interactive_text_ = nullptr;
  is_highlighted_ = false;
  top_left_x_ = 0;
  top_left_y_ = 0;
  width_ = text_->GetWidth() + text_padding_per_side_ * 2;
  height_ = text->GetHeight() + text_padding_per_side_ * 2;
}

InteractiveText::~InteractiveText() {
  return;
}

void InteractiveText::AddHighlight() {
  is_highlighted_ = true;
}

void InteractiveText::RemoveHighlight() {
  is_highlighted_ = false;
}

void InteractiveText::AddLink(InteractiveText *other) {

  if (other == nullptr) {
    printf("AddLink called with nullptr argument, not taking action\n");
    return;
  }

  if (linked_interactive_text_ == nullptr) {
    linked_interactive_text_ = other;
    linked_interactive_text_->AddLink(this);
  }

}

void InteractiveText::RemoveLink() {

  if (linked_interactive_text_ != nullptr) {
    linked_interactive_text_ = nullptr;
    linked_interactive_text_->RemoveLink();
  }

}

InteractiveText *InteractiveText::GetLink() {
  return linked_interactive_text_;
}

void InteractiveText::SetTopLeftPosition(int x, int y) {
  top_left_x_ = x;
  top_left_y_ = y;
}

void InteractiveText::Render() {

  SDL_Rect padding_rect = {top_left_x_, top_left_y_, width_, height_};

  if (is_highlighted_) {
    SDL_SetRenderDrawColor(renderer_, 0xDA, 0x70, 0xD6, 0xFF); // orchid
  } else {
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xCD, 0x58, 0xFF); // black
  }

  SDL_RenderFillRect(renderer_, &padding_rect);
  text_->Render(top_left_x_ + text_padding_per_side_, top_left_y_ + text_padding_per_side_);

}

void InteractiveText::HandleEvent(SDL_Event *event) {

  if (event->type != SDL_MOUSEBUTTONDOWN) {
    return;
  }

  int mouse_x, mouse_y;
  SDL_GetMouseState(&mouse_x, &mouse_y);

  bool mouse_inside_button = (mouse_x >= top_left_x_)
      && (mouse_x <= top_left_x_ + width_)
      && (mouse_y >= top_left_y_)
      && (mouse_y <= top_left_y_ + height_);

  if (mouse_inside_button) {

    switch (event->type) {
      case SDL_MOUSEBUTTONDOWN:
        if (is_highlighted_) {
          this->RemoveHighlight();
        } else {
          this->AddHighlight();
        }
        printf("Mouse down on the button with text: %s\n", text_->GetString().c_str());
        break;
      default:
        printf("Unrecognized mouse event type: %d", event->type);
        return;

    }
  }
}

Text *InteractiveText::GetText() {
  return text_;
}

}