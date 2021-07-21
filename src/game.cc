#include <SDL2/SDL.h>
#include <fmt/format.h>
#include "game.h"
#include "game_helper.h"
#include "button.h"
#include "interactive_text.h"
#include "word_pair_file_loader.h"

namespace cross_language_match {

Game::Game() {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error(fmt::format("SDL could not initialize, error: {}\n", SDL_GetError()));
  }

  window_ = SDL_CreateWindow("Cross Language Match",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             1080,
                             800,
                             SDL_WINDOW_SHOWN);

  if (window_ == nullptr) {
    throw std::runtime_error(fmt::format("Window could not be created, error: {}\n", SDL_GetError()));
  }

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

  if (renderer_ == nullptr) {
    throw std::runtime_error(fmt::format("Renderer could not be created, error: {}\n", SDL_GetError()));
  }
  SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);

  if (TTF_Init() == -1) {
    throw std::runtime_error(fmt::format("SDL_ttf could not be initialized, error: {}\n", TTF_GetError()));
  }

  font_ = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 28);

  if (font_ == NULL) {
    throw std::runtime_error(fmt::format("Failed to load font, error: {}\n", TTF_GetError()));
  }

}

Game::~Game() {

  SDL_DestroyWindow(window_);
  window_ = nullptr;
  TTF_CloseFont(font_);
  font_ = nullptr;
  TTF_Quit();
  SDL_Quit();

  delete correct_word_pair_strings_;

  for (auto &word : *left_words_) {
    if (word != nullptr) {
      delete word->GetText();
    }
    delete word;
  }
  delete left_words_;

  for (auto &word : *right_words_) {
    if (word != nullptr) {
      delete word->GetText();
    }
    delete word;
  }
  delete right_words_;

  // all_words_ is either:
  //   nullptr already (if LoadWords was invoked)
  //   OR it holds all of the pointers already contained in left_words_ and right_words_ (if LoadWords was invoked)
  // In neither case do we want to invoke delete on the contents of it; hence, we just delete the container
  delete all_words_;

}

void Game::LoadWords(std::string file_path) {

  if (correct_word_pair_strings_ != nullptr) {
    printf("LoadWords has already been invoked; this is thus a no-op. Use a new Game object in order to load a"
           " different word set");
    return;
  }

  correct_word_pair_strings_ = WordPairFileLoader::GetWordPairs(file_path);

  left_words_ = new std::vector<InteractiveText *>();
  right_words_ = new std::vector<InteractiveText *>();
  for (auto &word_pair_string : *correct_word_pair_strings_) {
    left_words_->push_back(
        new InteractiveText(renderer_, new Text(renderer_, font_, text_color_, word_pair_string.first), LEFT)
    );
    right_words_->push_back(
        new InteractiveText(renderer_, new Text(renderer_, font_, text_color_, word_pair_string.second), RIGHT)
    );
  }

  // Shuffle so the words are not displayed at equal levels in the GUI
  GameHelper::Shuffle(left_words_);
  GameHelper::Shuffle(right_words_);

  all_words_ = GameHelper::GetUnifiedVector(left_words_, right_words_);

}

void Game::LoopDrawUntilQuit() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  Button *submit_button = new Button(renderer_, 50, 50);
  ButtonEvent submit_button_event = NONE;

  SDL_Event e;
  bool quit = false;
  while (!quit) {

    while (SDL_PollEvent(&e)) {

      if (e.type == SDL_QUIT) {
        quit = true;
      }

      for (auto &word : *all_words_) {
        word->HandleEvent(&e, *all_words_);
      }

      submit_button_event = submit_button->HandleEvent(&e);
      if (submit_button_event == PRESSED) {
        if (GameHelper::AreAllWordsLinkedAndCorrect(all_words_, correct_word_pair_strings_)) {
          printf("All matches are correct!\n");
        } else {
          printf("Incorrect; try again!\n");
        }

      }

    }

    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer_);

    int y = 100;
    for (auto &left_word : *left_words_) {
      left_word->SetTopLeftPosition(100, y);
      left_word->Render();
      y += 50;
    }
    y = 100;
    for (auto &right_word : *right_words_) {
      right_word->SetTopLeftPosition(200, y);
      right_word->Render();
      y += 50;
    }

    submit_button->SetTopLeftPosition(300, y + 100);
    submit_button->Render();

    SDL_RenderPresent(renderer_);

  }

  delete submit_button;

}

}