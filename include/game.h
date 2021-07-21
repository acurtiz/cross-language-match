#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <vector>
#include <string>
#include <map>
#include <interactive_text.h>

#ifndef CROSSLANGUAGEMATCH_SRC_GAME_H_
#define CROSSLANGUAGEMATCH_SRC_GAME_H_

namespace cross_language_match {

class Game {

 public:
  Game();
  ~Game();
  void LoopDrawUntilQuit();

 private:

  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;
  TTF_Font *font_ = nullptr;
  SDL_Color text_color_ = {0, 0, 0};

  static bool AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> all_words,
                                          std::map<std::string, std::string> *expected_word_pairs);
  static void Shuffle(std::vector<InteractiveText *> *words);
  static std::vector<InteractiveText *> GetUnifiedVector(std::vector<InteractiveText *> *a,
                                                         std::vector<InteractiveText *> *b);

};

}
#endif //CROSSLANGUAGEMATCH_SRC_GAME_H_
