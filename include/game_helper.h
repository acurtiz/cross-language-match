#include <vector>
#include <map>
#include "interactive_text.h"

#ifndef CROSSLANGUAGEMATCH_SRC_GAME_HELPER_H_
#define CROSSLANGUAGEMATCH_SRC_GAME_HELPER_H_

namespace cross_language_match {

class GameHelper {
 private:
  friend class Game;
  static bool AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> *all_words,
                                          std::map<std::string, std::string> *expected_word_pairs);
  static void Shuffle(std::vector<InteractiveText *> *words);
  static std::vector<InteractiveText *> *GetUnifiedVector(std::vector<InteractiveText *> *a,
                                                          std::vector<InteractiveText *> *b);

};

}

#endif //CROSSLANGUAGEMATCH_SRC_GAME_HELPER_H_
