#include <vector>
#include "interactive_text.h"

namespace cross_language_match {

InteractiveText::InteractiveText(SDL_Renderer *renderer, Text *text, InteractiveTextGroup group) {
  renderer_ = renderer;
  text_ = text;
  linked_interactive_text_ = nullptr;
  is_highlighted_ = false;
  top_left_x_ = 0;
  top_left_y_ = 0;
  width_ = text_->GetWidth() + text_padding_per_side_ * 2;
  height_ = text->GetHeight() + text_padding_per_side_ * 2;
  group_ = group;
}

InteractiveText::~InteractiveText() {
  return;
}

void InteractiveText::AddHighlight() {

  printf("Adding highlight on %s\n", this->GetText()->GetString().c_str());
  is_highlighted_ = true;

}

void InteractiveText::RemoveHighlight() {

  printf("Removing highlight on %s\n", this->GetText()->GetString().c_str());
  is_highlighted_ = false;

}

void InteractiveText::AddLink(InteractiveText *other) {

  if (other == nullptr) {
    printf("AddLink called with nullptr argument, not taking action\n");
    return;
  }

  if (linked_interactive_text_ != nullptr) {
    printf("AddLink called but there is already a linked object; not taking action\n");
    return;
  }

  linked_interactive_text_ = other;

  printf("Linking object (%s) with object (%s)\n",
         this->GetText()->GetString().c_str(),
         this->GetLink()->GetText()->GetString().c_str());

}

void InteractiveText::RemoveLink() {

  if (linked_interactive_text_ == nullptr) {
    return;
  }

  printf("Removing link between (%s) and (%s)",
         this->GetText()->GetString().c_str(),
         this->GetLink()->GetText()->GetString().c_str());

  linked_interactive_text_ = nullptr;

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

  if (linked_interactive_text_ != nullptr) {
    SDL_SetRenderDrawColor(renderer_, 0xDA, 0x70, 0xD6, 0xFF); // orchid
    SDL_RenderDrawLine(renderer_,
                       top_left_x_ + width_ / 2,
                       top_left_y_ + height_ / 2,
                       linked_interactive_text_->top_left_x_ + linked_interactive_text_->width_ / 2,
                       linked_interactive_text_->top_left_y_ + linked_interactive_text_->height_ / 2);
  }

  if (is_highlighted_) {
    SDL_SetRenderDrawColor(renderer_, 0xDA, 0x70, 0xD6, 0xFF); // orchid
  } else {
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xCD, 0x58, 0xFF); // black
  }

  SDL_RenderFillRect(renderer_, &padding_rect);

  text_->Render(top_left_x_ + text_padding_per_side_, top_left_y_ + text_padding_per_side_);

}

void InteractiveText::HandleEvent(SDL_Event *event, std::vector<InteractiveText *> all_words) {

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

        if (event->button.button == SDL_BUTTON_LEFT) {

          if (linked_interactive_text_ != nullptr) {

            printf("Left click detected but object (%s) already has linked text (%s), doing nothing.\n",
                   this->text_->GetString().c_str(), this->linked_interactive_text_->text_->GetString().c_str());

          } else if (is_highlighted_) {

            printf("Left click detected but object (%s) already highlighted, doing nothing.\n",
                   this->text_->GetString().c_str());

          } else { // Not linked and not highlighted

            this->AddHighlight();

            // Find if any of the already existing words from a different group are already highlighted; if so, link them and remove highlighting.
            InteractiveText *existing_highlighted_from_other_group = GetHighlightedOtherFromDifferentGroup(all_words);
            if (existing_highlighted_from_other_group != nullptr) {
              this->RemoveHighlight();
              existing_highlighted_from_other_group->RemoveHighlight();
              this->AddLink(existing_highlighted_from_other_group);
              existing_highlighted_from_other_group->AddLink(this);
            }

            // ELSE, find if any already existing words from this group are already highlighted; if so, remove that other object's highlighting.
            InteractiveText *existing_highlighted_from_same_group = GetHighlightedOtherFromSameGroup(all_words);
            if (existing_highlighted_from_same_group != nullptr) {
              existing_highlighted_from_same_group->RemoveHighlight();
            }

          }

        } else if (event->button.button == SDL_BUTTON_RIGHT) {

          printf("Right click detected on highlighted text (%s); removing links and highlighting\n",
                 this->GetText()->GetString().c_str());

          if (this->GetLink() != nullptr) {
            this->GetLink()->RemoveHighlight();
            this->GetLink()->RemoveLink();
          }

          this->RemoveHighlight();
          this->RemoveLink();

        }

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

InteractiveTextGroup InteractiveText::GetGroup() {
  return group_;
}

InteractiveText *InteractiveText::GetHighlightedOtherFromSameGroup(std::vector<InteractiveText *> all_text) {

  for (auto &single_text : all_text) {

    if (single_text == this) {
      continue;
    }

    if (single_text->is_highlighted_ && single_text->group_ == this->group_) {
      printf("Current object (%s); found other text from same group (%s) which is highlighted)\n",
             this->GetText()->GetString().c_str(),
             single_text->GetText()->GetString().c_str());
      return single_text;
    }

  }

  return nullptr;

}

InteractiveText *InteractiveText::GetHighlightedOtherFromDifferentGroup(std::vector<InteractiveText *> all_text) {

  for (InteractiveText *single_text : all_text) {

    if (single_text == this) {
      continue;
    }

    if (single_text->is_highlighted_ && single_text->group_ != this->group_) {
      printf("Current object (%s); found other text from different group (%s) which is highlighted)\n",
             this->GetText()->GetString().c_str(),
             single_text->GetText()->GetString().c_str());
      return single_text;
    }

  }

  return nullptr;

}

}