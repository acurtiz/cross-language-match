#include <vector>
#include <cmath>
#include "text/interactive_text.h"
#include "button/cancellation_circle_button.h"

namespace cross_language_match {

InteractiveText::InteractiveText(SDL_Renderer *renderer, Text *text, InteractiveTextGroup group)
    : Rectangle(renderer,
                text->GetWidth() + text_padding_per_side_ * 2,
                text->GetHeight() + text_padding_per_side_ * 2),
      renderer_(renderer),
      text_(text),
      linked_interactive_text_(nullptr),
      is_highlighted_(false),
      group_(group),
      interactive_line_color_({0x48, 0x3C, 0x32, 0xFF}),
      interactive_text_highlight_color_({0x4E, 0xC3, 0x3D, 0xFF}),
      interactive_text_non_highlight_color_({0x48, 0x3C, 0x32, 0xFF}),
      link_cancellation_circle_(nullptr),
      line_one_x1_(0), line_one_x2_(0), line_one_y1_(0), line_one_y2_(0),
      line_two_x1_(0), line_two_x2_(0), line_two_y1_(0), line_two_y2_(0) {}

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

  line_one_x1_ = left_interactive_text->GetTopLeftX() + left_interactive_text->GetWidth();
  line_one_y1_ = left_interactive_text->GetTopLeftY() + left_interactive_text->GetHeight() / 2;
  line_one_x2_ = offset_x + circle_x_left_side;
  line_one_y2_ = offset_y + circle_y_left_side;

  line_two_x1_ = offset_x + circle_x_right_side;
  line_two_y1_ = offset_y + circle_y_right_side;
  line_two_x2_ = right_interactive_text->GetTopLeftX();
  line_two_y2_ = right_interactive_text->GetTopLeftY() + right_interactive_text->GetHeight() / 2;

  link_cancellation_circle_ = new CancellationCircleButton(renderer_);
  link_cancellation_circle_->SetRadius(circle_radius);
  link_cancellation_circle_->SetCenter(offset_x + circle_center_x, offset_y + circle_center_y);
  link_cancellation_circle_->SetColor({interactive_line_color_.r,
                                       interactive_line_color_.g,
                                       interactive_line_color_.b,
                                       interactive_line_color_.a});
}

void InteractiveText::RemoveLink() {

  if (linked_interactive_text_ == nullptr) {
    return;
  }

  linked_interactive_text_ = nullptr;

  delete link_cancellation_circle_;
  link_cancellation_circle_ = nullptr;

}

InteractiveText *InteractiveText::GetLink() {
  return linked_interactive_text_;
}

void InteractiveText::Render() {

  if (linked_interactive_text_ != nullptr) {
    SDL_SetRenderDrawColor(renderer_,
                           interactive_line_color_.r,
                           interactive_line_color_.g,
                           interactive_line_color_.b,
                           interactive_line_color_.a);
    SDL_RenderDrawLine(renderer_, line_one_x1_, line_one_y1_, line_one_x2_, line_one_y2_);
    link_cancellation_circle_->Render();
    SDL_RenderDrawLine(renderer_, line_two_x1_, line_two_y1_, line_two_x2_, line_two_y2_);
  }

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

void InteractiveText::SetTopLeftPosition(int top_left_x, int top_left_y) {

  Rectangle::SetTopLeftPosition(top_left_x, top_left_y);
  text_->SetTopLeftPosition(GetTopLeftX() + text_padding_per_side_,
                            GetTopLeftY() + text_padding_per_side_);

}

void InteractiveText::HandleEvent(SDL_Event *event, std::vector<InteractiveText *> all_words) {

  if (link_cancellation_circle_ != nullptr && link_cancellation_circle_->HandleEvent(event) == PRESSED) {

    if (this->GetLink() != nullptr) {
      this->GetLink()->RemoveHighlight();
      this->GetLink()->RemoveLink();
    }

    this->RemoveHighlight();
    this->RemoveLink();

  }

  if (IsMouseInside() && event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {

    if (GetLink() != nullptr) {

      // Do nothing if already linked

    } else if (is_highlighted_) {

      RemoveHighlight();

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