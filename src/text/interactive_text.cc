#include <vector>
#include "text/interactive_text.h"

namespace cross_language_match {

InteractiveText::InteractiveText(SDL_Renderer *renderer, Text *text, InteractiveTextGroup group)
    : Rectangle(renderer) {

  renderer_ = renderer;
  text_ = text;
  linked_interactive_text_ = nullptr;
  is_highlighted_ = false;
  group_ = group;

  Rectangle::SetTopLeftPosition(0, 0);
  Rectangle::SetWidth(text_->GetWidth() + text_padding_per_side_ * 2);
  Rectangle::SetHeight(text_->GetHeight() + text_padding_per_side_ * 2);

}

void InteractiveText::AddHighlight() {
  is_highlighted_ = true;
}

void InteractiveText::RemoveHighlight() {
  is_highlighted_ = false;
}

void InteractiveText::AddLink(InteractiveText *other) {

  if (other == nullptr) {
    return;
  }

  if (linked_interactive_text_ != nullptr) {
    return;
  }

  linked_interactive_text_ = other;

}

void InteractiveText::RemoveLink() {

  if (linked_interactive_text_ == nullptr) {
    return;
  }

  linked_interactive_text_ = nullptr;

}

InteractiveText *InteractiveText::GetLink() {
  return linked_interactive_text_;
}

void InteractiveText::Render() {

  // The line should be drawn from the middle of the right edge of the *left* word to the middle of the left edge
  // of the right word
  InteractiveText *left_interactive_text;
  InteractiveText *right_interactive_text;
  if (group_ == LEFT) {
    left_interactive_text = this;
    right_interactive_text = linked_interactive_text_;
  } else {
    left_interactive_text = linked_interactive_text_;
    right_interactive_text = this;
  }

  if (linked_interactive_text_ != nullptr) {
    SDL_SetRenderDrawColor(renderer_,
                           interactive_line_color_.r,
                           interactive_line_color_.g,
                           interactive_line_color_.b,
                           interactive_line_color_.a);
    SDL_RenderDrawLine(renderer_,
                       left_interactive_text->GetTopLeftX() + left_interactive_text->GetWidth(),
                       left_interactive_text->GetTopLeftY() + left_interactive_text->GetHeight() / 2,
                       right_interactive_text->GetTopLeftX(),
                       right_interactive_text->GetTopLeftY() + right_interactive_text->GetHeight() / 2);
  }

  if (is_highlighted_) {
    SetColor({
                 interactive_text_highlight_color_.r,
                 interactive_text_highlight_color_.g,
                 interactive_text_highlight_color_.b,
                 interactive_text_highlight_color_.a
             });
  } else {
    SetColor({
                 interactive_text_non_highlight_color_.r,
                 interactive_text_non_highlight_color_.b,
                 interactive_text_non_highlight_color_.g,
                 interactive_text_non_highlight_color_.a
             });
  }

  Rectangle::Render();
  text_->Render();

}

void InteractiveText::SetTopLeftPosition(int top_left_x, int top_left_y) {

  Rectangle::SetTopLeftPosition(top_left_x, top_left_y);
  text_->SetTopLeftPosition(GetTopLeftX() + text_padding_per_side_,
                            GetTopLeftY() + text_padding_per_side_);

}

void InteractiveText::HandleEvent(SDL_Event *event, std::vector<InteractiveText *> all_words) {

  if (event->type != SDL_MOUSEBUTTONDOWN) {
    return;
  }

  if (IsMouseInside()) {

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

int InteractiveText::GetPaddingPerSide() {
  return text_padding_per_side_;
}

}