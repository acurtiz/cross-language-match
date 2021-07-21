#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include <map>
#include "interactive_text.h"

#ifndef CROSSLANGUAGEMATCH_SRC_GAME_H_
#define CROSSLANGUAGEMATCH_SRC_GAME_H_

namespace cross_language_match {

class Game {

 public:
  Game();
  ~Game();
  void LoadWordPairs(std::string file_path);
  void LoopDrawUntilQuit();

 private:

  void PrepareCurrentWords();
  void CleanCurrentWords();

  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
  TTF_Font *font_ = nullptr;
  SDL_Color text_color_ = {0, 0, 0};

  std::vector<InteractiveText *> *left_words_ = nullptr;
  std::vector<InteractiveText *> *right_words_ = nullptr;
  std::vector<InteractiveText *> *left_and_right_words_ = nullptr;
  std::map<std::string, std::string> *remaining_word_pairs_ = nullptr;
  std::map<std::string, std::string> *current_word_pairs_ = nullptr;

  const int words_to_present_per_round_ = 10;

};

}
#endif //CROSSLANGUAGEMATCH_SRC_GAME_H_
