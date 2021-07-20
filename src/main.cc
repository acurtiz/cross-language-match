#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <map>
#include <vector>
#include <fmt/core.h>

#include <text.h>
#include <interactive_text.h>
#include <word_pair_file_loader.h>
#include <button.h>

namespace cross_language_match {

void Init();
void Close();
void Draw();
bool AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> all_words,
                                 std::map<std::string, std::string> *expected_word_pairs);

SDL_Window *kWindow = nullptr;
SDL_Renderer *kRenderer = nullptr;
TTF_Font *kFont = nullptr;
SDL_Color kTextColor = {0, 0, 0};

void Init() {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error(fmt::format("SDL could not initialize, error: {}\n", SDL_GetError()));
  }

  kWindow = SDL_CreateWindow("Cross Language Match",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             1080,
                             800,
                             SDL_WINDOW_SHOWN);

  if (kWindow == nullptr) {
    throw std::runtime_error(fmt::format("Window could not be created, error: {}\n", SDL_GetError()));
  }

  kRenderer = SDL_CreateRenderer(kWindow, -1, SDL_RENDERER_ACCELERATED);

  if (kRenderer == nullptr) {
    throw std::runtime_error(fmt::format("Renderer could not be created, error: {}\n", SDL_GetError()));
  }
  SDL_SetRenderDrawColor(kRenderer, 0x00, 0x00, 0x00, 0xFF);

  if (TTF_Init() == -1) {
    throw std::runtime_error(fmt::format("SDL_ttf could not be initialized, error: {}\n", TTF_GetError()));
  }

  kFont = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 28);

  if (kFont == NULL) {
    throw std::runtime_error(fmt::format("Failed to load font, error: {}\n", TTF_GetError()));
  }

}

void Close() {

  SDL_DestroyWindow(kWindow);
  kWindow = nullptr;
  TTF_CloseFont(kFont);
  kFont = nullptr;
  TTF_Quit();
  SDL_Quit();

}

void Draw() {

  SDL_SetRenderDrawColor(kRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(kRenderer);
  SDL_RenderPresent(kRenderer);

  SDL_Event e;
  bool quit = false;

  WordPairFileLoader file_loader = WordPairFileLoader("assets/txt/test-pairs.csv");
  std::map<std::string, std::string> *word_pair_strings = file_loader.GetWordPairs();

  std::vector<InteractiveText *> left_words;
  std::vector<InteractiveText *> right_words;
  for (auto &word_pair_string : *word_pair_strings) {
    left_words.push_back(
        new InteractiveText(kRenderer, new Text(kRenderer, kFont, kTextColor, word_pair_string.first), LEFT)
    );
    right_words.push_back(
        new InteractiveText(kRenderer, new Text(kRenderer, kFont, kTextColor, word_pair_string.second), RIGHT)
    );
  }

  std::vector<InteractiveText *> all_words;
  all_words.insert(all_words.end(), left_words.begin(), left_words.end());
  all_words.insert(all_words.end(), right_words.begin(), right_words.end());

  Button *submit_button = new Button(kRenderer, 50, 50);
  ButtonEvent submit_button_event = NONE;

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
        if (AreAllWordsLinkedAndCorrect(all_words, word_pair_strings)) {
          printf("All matches are correct!\n");
        } else {
          printf("Incorrect; try again!\n");
        }

      }

    }

    // Clear the screen (set it to white)
    SDL_SetRenderDrawColor(kRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(kRenderer);

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

    SDL_RenderPresent(kRenderer);

  }

}

bool AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> all_words,
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

int main() {
  cross_language_match::Init();
  cross_language_match::Draw();
  cross_language_match::Close();
  return 0;
}
