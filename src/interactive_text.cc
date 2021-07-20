//
// Created by Alex Curtis on 7/20/21.
//

#include "interactive_text.h"

namespace cross_language_match {

InteractiveText::InteractiveText(SDL_Renderer *renderer, Text *text) {
  renderer_ = renderer;
  text_ = text;
  is_highlighted_ = false;
  linked_interactive_text_ = nullptr;
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

void InteractiveText::Render(int x, int y) {

  int padding_per_side = 5;
  const SDL_Rect padding_rect = {x - padding_per_side,
                                 y - padding_per_side,
                                 text_->GetWidth() + padding_per_side * 2,
                                 text_->GetHeight() + padding_per_side * 2};

  if (is_highlighted_) {
    SDL_SetRenderDrawColor(renderer_, 0xDA, 0x70, 0xD6, 0xFF); // orchid
  } else {
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xCD, 0x58, 0xFF); // black
  }

  SDL_RenderFillRect(renderer_, &padding_rect);
  text_->Render(x, y);

}

Text *InteractiveText::GetText() {
  return text_;
}

}