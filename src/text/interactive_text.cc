#include <vector>
#include <cmath>
#include "text/interactive_text.h"
#include "shape/circle_with_x.h"

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

  RenderLink();

  if (is_highlighted_) {
    Rectangle::SetColor({
                            interactive_text_highlight_color_.r,
                            interactive_text_highlight_color_.g,
                            interactive_text_highlight_color_.b,
                            interactive_text_highlight_color_.a
                        });
  } else {
    Rectangle::SetColor({
                            interactive_text_non_highlight_color_.r,
                            interactive_text_non_highlight_color_.b,
                            interactive_text_non_highlight_color_.g,
                            interactive_text_non_highlight_color_.a
                        });
  }

  Rectangle::Render();
  text_->Render();

}

void InteractiveText::RenderLink() {

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

    // We calculate the slope of the line from the left interactive text object to the right interactive text object
    double y_delta = (right_interactive_text->GetTopLeftY() + (double) right_interactive_text->GetHeight() / 2)
        - (left_interactive_text->GetTopLeftY() + (double) left_interactive_text->GetHeight() / 2);
    double x_delta = (right_interactive_text->GetTopLeftX())
        - (left_interactive_text->GetTopLeftX() + left_interactive_text->GetWidth());
    double slope = y_delta / x_delta;

    // For a moment we assume the origin is the right, middle side of the *left* interactive text object
    // We fix the center of the circle to be 30 pixels along from on the X-axis along that line, and we
    // calculate what the corresponding y coordinate should be to be along that line. This will be the
    // center of the circle we draw
    int circle_center_x = 30;
    int circle_center_y = (int) (slope * circle_center_x);

    // We had assumed the origin wasn't the top left of the screen, so we must translate since to SDL, it is
    int offset_x = left_interactive_text->GetTopLeftX() + left_interactive_text->GetWidth();
    int offset_y = left_interactive_text->GetTopLeftY() + left_interactive_text->GetHeight() / 2;

    int circle_radius = 10;

    // We don't want to draw our linking line through the circle; we want to have a line from the interactive text
    // object until it hits the circle, and then another line to continue on the same slope after the circle; thus,
    // we must calculate what those intersecting points are on the left and right side of the circle
    // In order to do that, we'll actually calculate as accurately as we can, but we could use a heuristic to simplify
    // if necessary in the future.
    int circle_x_left_side =
        (int) sqrt((pow(sqrt(pow(circle_center_x, 2) + pow(circle_center_y, 2)) - circle_radius, 2)
            - pow(circle_center_y, 2) / pow(circle_center_x, 2)) / (1 + 1 / pow(circle_center_x, 2)));
    int circle_y_left_side = circle_x_left_side * circle_center_y / circle_center_x;
    int circle_x_right_side = circle_center_x + (circle_center_x - circle_x_left_side);
    int circle_y_right_side = circle_center_y + (circle_center_y - circle_y_left_side);

    // Draw a line from the left interactive text to the left side of the circle
    SDL_RenderDrawLine(renderer_,
                       left_interactive_text->GetTopLeftX() + left_interactive_text->GetWidth(),
                       left_interactive_text->GetTopLeftY() + left_interactive_text->GetHeight() / 2,
                       offset_x + circle_x_left_side,
                       offset_y + circle_y_left_side);

    // Draw the circle
    CircleWithX circle = CircleWithX(renderer_);
    circle.SetRadius(circle_radius);
    circle.SetCenter(offset_x + circle_center_x, offset_y + circle_center_y);
    circle.SetColor({interactive_line_color_.r,
                     interactive_line_color_.g,
                     interactive_line_color_.b,
                     interactive_line_color_.a});
    circle.Render();

    // Draw a line from the right side of the circle to the right interactive text
    SDL_RenderDrawLine(renderer_,
                       offset_x + circle_x_right_side,
                       offset_y + circle_y_right_side,
                       right_interactive_text->GetTopLeftX(),
                       right_interactive_text->GetTopLeftY() + right_interactive_text->GetHeight() / 2);

  }
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