#include <SDL2/SDL.h>
#include <boost/format.hpp>
#include "game.h"
#include "game_helper.h"
#include "button.h"
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
  GameHelper::Shuffle(left_words_);
  GameHelper::Shuffle(right_words_);

  left_and_right_words_ = GameHelper::GetUnifiedVector(left_words_, right_words_);

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

  Button *submit_button = new Button(renderer_, submit_button_width_, submit_button_height_);
  ButtonEvent submit_button_event = NONE;

  SDL_Event e;
  bool quit = false;

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

        if (GameHelper::AreAllWordsLinkedAndCorrect(left_and_right_words_, current_word_pairs_)) {

          if (remaining_word_pairs_->empty()) {
            printf("Correct! Game is over! All words done!\n");
            quit = true;
          } else {
            printf("Correct! Preparing next set of words!\n");
            PrepareCurrentWords();
          }

        } else {
          printf("Incorrect; try again!\n");
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

    submit_button->SetTopLeftPosition(padding_individual_words_, screen_height_ * 0.9);
    submit_button->Render();

    SDL_RenderPresent(renderer_);

  }

  delete submit_button;

}

}