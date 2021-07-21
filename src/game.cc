
#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <random>

#include "game.h"
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

}

void Game::LoopDrawUntilQuit() {

  SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(renderer_);
  SDL_RenderPresent(renderer_);

  // Load and parse CSV into map
  WordPairFileLoader *file_loader = new WordPairFileLoader("assets/txt/test-pairs.csv");
  std::map<std::string, std::string> word_pair_strings = file_loader->GetWordPairs();
  delete file_loader;

  // Create InteractiveText objects for left and right side from map
  std::vector<InteractiveText *> left_words;
  std::vector<InteractiveText *> right_words;
  for (auto &word_pair_string : word_pair_strings) {
    left_words.push_back(
        new InteractiveText(renderer_, new Text(renderer_, font_, text_color_, word_pair_string.first), LEFT)
    );
    right_words.push_back(
        new InteractiveText(renderer_, new Text(renderer_, font_, text_color_, word_pair_string.second), RIGHT)
    );
  }

  // Shuffle so the words are not displayed at equal levels in the GUI
  Shuffle(&left_words);
  Shuffle(&right_words);

  // Get a convenience unified vector for iteration over all words
  std::vector<InteractiveText *> all_words = GetUnifiedVector(&left_words, &right_words);

  Button *submit_button = new Button(renderer_, 50, 50);
  ButtonEvent submit_button_event = NONE;

  SDL_Event e;
  bool quit = false;
  while (!quit) {

    while (SDL_PollEvent(&e)) {

      if (e.type == SDL_QUIT) {
        quit = true;
      }

      for (auto &word : all_words) {
        word->HandleEvent(&e, all_words);
      }

      submit_button_event = submit_button->HandleEvent(&e);
      if (submit_button_event == PRESSED) {
        if (AreAllWordsLinkedAndCorrect(all_words, &word_pair_strings)) {
          printf("All matches are correct!\n");
        } else {
          printf("Incorrect; try again!\n");
        }

      }

    }

    // Reset the screen to white
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer_);

    // Render the words
    int y = 100;
    for (auto &left_word : left_words) {
      left_word->SetTopLeftPosition(100, y);
      left_word->Render();
      y += 50;
    }
    y = 100;
    for (auto &right_word : right_words) {
      right_word->SetTopLeftPosition(200, y);
      right_word->Render();
      y += 50;
    }

    // Render the button
    submit_button->SetTopLeftPosition(300, y + 100);
    submit_button->Render();

    SDL_RenderPresent(renderer_);

  }

}

void Game::Shuffle(std::vector<InteractiveText *> *words) {

  std::shuffle(words->begin(),
               words->end(),
               std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())
  );

}

std::vector<InteractiveText *> Game::GetUnifiedVector(std::vector<InteractiveText *> *a,
                                                      std::vector<InteractiveText *> *b) {

  std::vector<InteractiveText *> all_words;
  all_words.insert(all_words.end(), a->begin(), a->end());
  all_words.insert(all_words.end(), b->begin(), b->end());
  return all_words;

}

bool Game::AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> all_words,
                                       std::map<std::string, std::string> *expected_word_pairs) {

  // Ensure all words have been paired up
  for (auto &word : all_words) {
    if (word->GetLink() == nullptr) {
      printf("Word (%s) is not paired up; all words must be paired up for submission to occur.\n",
             word->GetText()->GetString().c_str());
      return false;
    }
  }

  for (auto &word : all_words) {

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