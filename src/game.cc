#include <SDL2/SDL.h>
#include <boost/format.hpp>
#include "game.h"
#include "game_utility.h"
#include "button/button.h"
#include "button/labeled_button.h"
#include "interactive_text.h"
#include "word_loader/file_word_loader.h"
#include "word_loader/string_word_loader.h"

namespace cross_language_match {

Game::Game() {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error(
        boost::str(boost::format("SDL could not initialize, error: %1\n") % SDL_GetError())
    );
  }

  window_ = SDL_CreateWindow("Cross Language Match",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             screen_width_,
                             screen_height_,
                             SDL_WINDOW_SHOWN);

  if (window_ == nullptr) {
    throw std::runtime_error(
        boost::str(boost::format("Window could not be created, error: %1\n") % SDL_GetError())
    );
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

  if (renderer_ == nullptr) {
    throw std::runtime_error(
        boost::str(boost::format("Renderer could not be created, error: %1\n") % SDL_GetError())
    );
  }
  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

  if (TTF_Init() == -1) {
    throw std::runtime_error(
        boost::str(boost::format("SDL_ttf could not be initialized, error: %1\n") % TTF_GetError())
    );
  }

  font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", font_size_);

  if (font_ == NULL) {
    throw std::runtime_error(boost::str(boost::format("Failed to load font, error: %1\n") % TTF_GetError()));
  }

}

Game::~Game() {

  SDL_DestroyWindow(window_);
  window_ = nullptr;
  TTF_CloseFont(font_);
  font_ = nullptr;
  TTF_Quit();
  SDL_Quit();

  CleanCurrentWords();

  delete remaining_word_pairs_;
}

void Game::LoadWordsViaString(std::string raw_string) {

  if (remaining_word_pairs_ != nullptr) {
    printf("LoadWordsViaFile has already been invoked; this is thus a no-op. Use a new Game object in order to load a"
           " different word set");
    return;
  }

  StringWordLoader word_loader = StringWordLoader(raw_string);
  remaining_word_pairs_ = word_loader.GetWordPairs();

}

void Game::LoadWordsViaFile(std::string file_path) {

  if (remaining_word_pairs_ != nullptr) {
    printf("LoadWordsViaFile has already been invoked; this is thus a no-op. Use a new Game object in order to load a"
           " different word set");
    return;
  }

  FileWordLoader word_pair_file_loader = FileWordLoader(file_path);
  remaining_word_pairs_ = word_pair_file_loader.GetWordPairs();

}

void Game::PrepareCurrentWords() {

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
  GameUtility::Shuffle(left_words_);
  GameUtility::Shuffle(right_words_);

  left_and_right_words_ = GameUtility::GetUnifiedVector(left_words_, right_words_);

}

void Game::CleanCurrentWords() {

  if (left_words_ != nullptr) {
    for (auto &word : *left_words_) {
      if (word != nullptr) {
        delete word->GetText();
      }
      delete word;
    }
    delete left_words_;
  }

  if (right_words_ != nullptr) {
    for (auto &word : *right_words_) {
      if (word != nullptr) {
        delete word->GetText();
      }
      delete word;
    }
    delete right_words_;
  }

  // left_and_right_words_ is either:
  //   nullptr already (if LoadWordsViaFile was invoked)
  //   OR it holds all of the pointers already contained in left_words_ and right_words_ (if LoadWordsViaFile was invoked)
  // In neither case do we want to invoke delete on the contents of it; hence, we just delete the container
  delete left_and_right_words_;

  delete current_word_pairs_;

}

void Game::LoopDrawUntilQuit() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  PrepareCurrentWords();

  Text *incorrect_text = new Text(renderer_, font_, text_color_, "Incorrect; please try again.");
  Text *correct_text = new Text(renderer_, font_, text_color_, "Correct - well done!");

  Text *submit_text = new Text(renderer_, font_, text_color_, "Submit");
  Button *submit_button = new LabeledButton(renderer_, submit_button_width_, submit_button_height_, submit_text);
  Text *next_round_text = new Text(renderer_, font_, text_color_, "Next Round");
  Button *next_round_button = new LabeledButton(renderer_, next_button_width_, next_button_height_, next_round_text);

  ButtonEvent submit_button_event = NONE;
  ButtonEvent next_round_button_event = NONE;

  SDL_Event e;
  bool quit = false;
  bool all_rounds_complete = false;
  bool current_round_is_complete = false;
  bool last_submission_was_incorrect = false;

  while (!quit) {

    while (SDL_PollEvent(&e)) {

      if (e.type == SDL_QUIT) {
        quit = true;
      }

      for (auto &word : *left_and_right_words_) {
        word->HandleEvent(&e, *left_and_right_words_);
      }

      submit_button_event = submit_button->HandleEvent(&e);

      if (submit_button_event == PRESSED) {
        if (GameUtility::AreAllWordsLinkedAndCorrect(left_and_right_words_, current_word_pairs_)) {
          printf("Correct! Preparing next set of words!\n");
          last_submission_was_incorrect = false;
          current_round_is_complete = true;
        } else {
          last_submission_was_incorrect = true;
          // In case the submit button is pressed *after* the set has been matched correctly, the user has undid the
          // completion
          current_round_is_complete = false;
          printf("Incorrect; try again!\n");
        }
      }

      if (current_round_is_complete && remaining_word_pairs_->empty()) {
        printf("Correct! Game is over! All words done!\n");
        all_rounds_complete = true;
      }

      next_round_button_event = next_round_button->HandleEvent(&e);
      if (next_round_button_event == PRESSED) {
        if (all_rounds_complete) {
          quit = true;
        } else {
          PrepareCurrentWords();
          current_round_is_complete = false;
        }
      }

    }

    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer_);

    // Render left column
    int y = padding_individual_words_;
    for (auto &left_word : *left_words_) {
      left_word->SetTopLeftPosition(padding_word_columns_, y);
      left_word->Render();
      y += left_word->GetHeight() + padding_individual_words_;
    }

    // Render right column
    y = padding_individual_words_;
    for (auto &right_word : *right_words_) {
      right_word->SetTopLeftPosition(screen_width_ - padding_word_columns_ - right_word->GetWidth(), y);
      right_word->Render();
      y += right_word->GetHeight() + padding_individual_words_;
    }

    // Render submit button in bottom left
    submit_button->SetTopLeftPosition(10,
                                      screen_height_ - submit_button_height_ - 10);
    submit_button->Render();

    // Render next round button in bottom right, as well as a congratulations message in the middle
    if (current_round_is_complete) {
      next_round_button->SetTopLeftPosition(screen_width_ - 10 - next_button_width_,
                                            screen_height_ - next_button_height_ - 10);
      next_round_button->Render();
      correct_text->Render(screen_width_ / 2 - correct_text->GetWidth() / 2,
                           screen_height_ - correct_text->GetHeight() - 10);
    }

    // Render incorrect message
    if (last_submission_was_incorrect) {
      incorrect_text->Render(screen_width_ / 2 - incorrect_text->GetWidth() / 2,
                             screen_height_ - incorrect_text->GetHeight() - 10);
    }

    SDL_RenderPresent(renderer_);

  }

  delete submit_button;

}

}