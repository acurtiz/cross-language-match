#include <random>
#include "scene/game_scene.h"
#include "word_loader/string_word_loader.h"
#include "word_loader/file_word_loader.h"
#include "button/button.h"
#include "button/labeled_button.h"
#include "boost/format.hpp"

namespace cross_language_match {

GameScene::GameScene(SDL_Renderer *renderer,
                     SDL_Window *window,
                     bool &global_quit,
                     int screen_height,
                     int screen_width,
                     std::map<std::string, std::string> word_pairs)
    : Scene(renderer, window, global_quit),
      screen_height_(screen_height),
      screen_width_(screen_width) {

  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

  if (TTF_Init() == -1) {
    throw std::runtime_error(
        boost::str(boost::format("SDL_ttf could not be initialized, error: %1%\n") % TTF_GetError())
    );
  }

  font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", font_size_);

  if (font_ == NULL) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1%\n") % TTF_GetError()));
  }

  remaining_word_pairs_ = new std::map<std::string, std::string>(word_pairs);

}

GameScene::~GameScene() {

  TTF_CloseFont(font_);
  font_ = nullptr;
  TTF_Quit();

  delete remaining_word_pairs_;

  RunPostLoop();
  CleanCurrentWords();

}

void GameScene::RunPreLoop() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  PrepareCurrentWords();

  incorrect_text_ = new Text(renderer_, font_, text_color_, "Incorrect; please try again.");
  correct_text_ = new Text(renderer_, font_, text_color_, "Correct - well done!");

  submit_text_ = new Text(renderer_, font_, text_color_, "Submit");
  submit_button_ = new LabeledButton(renderer_, submit_button_width_, submit_button_height_, submit_text_);
  next_round_text_ = new Text(renderer_, font_, text_color_, "Next Round");
  next_round_button_ = new LabeledButton(renderer_, next_button_width_, next_button_height_, next_round_text_);
  return_text_ = new Text(renderer_, font_, text_color_, "Main Menu");
  return_button_ = new LabeledButton(renderer_, return_button_width_, return_button_height_, return_text_);
  submit_button_event_ = NONE;
  next_round_button_event_ = NONE;
  return_button_event_ = NONE;

}

void GameScene::RunPostLoop() {

  delete incorrect_text_;
  incorrect_text_ = nullptr;

  delete correct_text_;
  correct_text_ = nullptr;

  delete submit_text_;
  submit_text_ = nullptr;

  delete submit_button_;
  submit_button_ = nullptr;

  delete next_round_text_;
  next_round_text_ = nullptr;

  delete next_round_button_;
  next_round_button_ = nullptr;

  delete return_text_;
  return_text_ = nullptr;

  delete return_button_;
  return_button_ = nullptr;

  submit_button_event_ = NONE;
  next_round_button_event_ = NONE;
  return_button_event_ = NONE;

}

void GameScene::RunSingleIterationEventHandler(SDL_Event &event) {

  if (event.type == SDL_QUIT) {
    QuitGlobal();
  }

  for (auto &word : *left_and_right_words_) {
    word->HandleEvent(&event, *left_and_right_words_);
  }

  submit_button_event_ = submit_button_->HandleEvent(&event);

  if (submit_button_event_ == PRESSED) {
    if (AreAllWordsLinkedAndCorrect(left_and_right_words_, current_word_pairs_)) {
      printf("Correct! Preparing next set of words!\n");
      last_submission_was_incorrect_ = false;
      current_round_is_complete_ = true;
    } else {
      last_submission_was_incorrect_ = true;
      // In case the submit button is pressed *after* the set has been matched correctly, the user has undid the
      // completion
      current_round_is_complete_ = false;
      printf("Incorrect; try again!\n");
    }
  }

  if (current_round_is_complete_ && remaining_word_pairs_->empty()) {
    printf("Correct! Game is over! All words done!\n");
    all_rounds_complete_ = true;
  }

  next_round_button_event_ = next_round_button_->HandleEvent(&event);
  if (next_round_button_event_ == PRESSED) {
    if (all_rounds_complete_) {
      QuitLocal();
    } else {
      PrepareCurrentWords();
      current_round_is_complete_ = false;
    }
  }

  return_button_event_ = return_button_->HandleEvent(&event);
  if (return_button_event_ == PRESSED) {
    QuitLocal();
  }

}

void GameScene::RunSingleIterationLoopBody() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);

  // Render left column
  int y = padding_individual_words_;
  for (auto &left_word: *left_words_) {
    left_word->SetTopLeftPosition(padding_word_columns_, y);
    left_word->Render();
    y += left_word->GetHeight() + padding_individual_words_;
  }

  // Render right column
  y = padding_individual_words_;
  for (auto &right_word: *right_words_) {
    right_word->SetTopLeftPosition(screen_width_ - padding_word_columns_ - right_word->GetWidth(), y);
    right_word->Render();
    y += right_word->GetHeight() + padding_individual_words_;
  }

  // Render submit button in bottom left
  submit_button_->SetTopLeftPosition(10, screen_height_ - submit_button_height_ - 10);
  submit_button_->Render();

  // Render return button in the bottom right
  return_button_->SetTopLeftPosition(screen_width_ - 10 - return_button_width_,
                                     screen_height_ - return_button_height_ - 10);
  return_button_->Render();

  // Render next round button to the right of the submit button, as well as a congratulations message in the middle
  if (current_round_is_complete_) {

    if (!all_rounds_complete_) {
      next_round_button_->SetTopLeftPosition(10 + submit_button_width_ + 10,
                                             screen_height_ - next_button_height_ - 10);
      next_round_button_->Render();
    }

    correct_text_->Render(screen_width_ / 2 - correct_text_->GetWidth() / 2,
                          screen_height_ - correct_text_->GetHeight() - 10);
  }

  // Render incorrect message
  if (last_submission_was_incorrect_) {
    incorrect_text_->Render(screen_width_ / 2 - incorrect_text_->GetWidth() / 2,
                            screen_height_ - incorrect_text_->GetHeight() - 10);
  }

  SDL_RenderPresent(renderer_);

}

void GameScene::PrepareCurrentWords() {

  CleanCurrentWords();

  left_words_ = new std::vector<InteractiveText *>();
  right_words_ = new std::vector<InteractiveText *>();
  current_word_pairs_ = new std::map<std::string, std::string>();

  // Move a subset of words from remaining_correct_word_pairs to current_correct_word_pairs
  int remaining_count = words_to_present_per_round_;
  for (auto &word_pair : *remaining_word_pairs_) {
    if (remaining_count == 0) {
      break;
    }
    current_word_pairs_->insert(word_pair);
    remaining_count--;
  }
  for (auto &word_pair : *current_word_pairs_) {
    remaining_word_pairs_->erase(word_pair.first);
  }

  // Allocate new InteractiveText objects for current word pairs and add them to the vectors
  for (auto &word_pair : *current_word_pairs_) {
    left_words_->push_back(
        new InteractiveText(renderer_, new Text(renderer_, font_, text_color_, word_pair.first), LEFT)
    );
    right_words_->push_back(
        new InteractiveText(renderer_, new Text(renderer_, font_, text_color_, word_pair.second), RIGHT)
    );
  }

  // Shuffle so the words are not displayed at equal levels in the GUI
  Shuffle(left_words_);
  Shuffle(right_words_);

  left_and_right_words_ = GetUnifiedVector(left_words_, right_words_);

}

void GameScene::CleanCurrentWords() {

  if (left_words_ != nullptr) {
    for (auto &word : *left_words_) {
      if (word != nullptr) {
        delete word->GetText();
      }
      delete word;
    }
    delete left_words_;
    left_words_ = nullptr;
  }

  if (right_words_ != nullptr) {
    for (auto &word : *right_words_) {
      if (word != nullptr) {
        delete word->GetText();
      }
      delete word;
    }
    delete right_words_;
    right_words_ = nullptr;
  }

  // left_and_right_words_ is either:
  //   nullptr already
  //   OR it holds all of the pointers already contained in left_words_ and right_words_ (if LoadWordsViaFile was invoked)
  // In neither case do we want to invoke delete on the contents of it; hence, we just delete the container
  delete left_and_right_words_;
  left_and_right_words_ = nullptr;

  delete current_word_pairs_;
  current_word_pairs_ = nullptr;

}

void GameScene::Shuffle(std::vector<InteractiveText *> *vector) {

  std::shuffle(vector->begin(),
               vector->end(),
               std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())
  );

}

std::vector<InteractiveText *> *GameScene::GetUnifiedVector(std::vector<InteractiveText *> *a,
                                                            std::vector<InteractiveText *> *b) {

  std::vector<InteractiveText *> *all_words = new std::vector<InteractiveText *>();
  all_words->insert(all_words->end(), a->begin(), a->end());
  all_words->insert(all_words->end(), b->begin(), b->end());
  return all_words;

}

bool GameScene::AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> *all_words,
                                            std::map<std::string, std::string> *expected_word_pairs) {

  // Ensure all words have been paired up
  for (auto &word : *all_words) {
    if (word->GetLink() == nullptr) {
      printf("Word (%s) is not paired up; all words must be paired up for submission to occur.\n",
             word->GetText()->GetString().c_str());
      return false;
    }
  }

  for (auto &word : *all_words) {

    // Only need to inspect left words since they've been shown to be paired to all right words already
    if (word->GetGroup() != LEFT) {
      continue;
    }

    std::string left_word_str = word->GetText()->GetString();
    std::string actual_linked_right_word = word->GetLink()->GetText()->GetString();
    std::string expected_linked_right_word = (*expected_word_pairs)[left_word_str];

    if (actual_linked_right_word != expected_linked_right_word) {
      printf(
          "Left word (%s) is linked to right word (%s) but this is incorrect; expected it to be linked to (%s)\n",
          left_word_str.c_str(),
          actual_linked_right_word.c_str(),
          expected_linked_right_word.c_str());
      return false;
    }

  }

  return true;

}

}
