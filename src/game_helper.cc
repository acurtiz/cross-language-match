#include <vector>
#include <random>
#include "game_helper.h"
#include "interactive_text.h"

namespace cross_language_match {

void GameHelper::Shuffle(std::vector<InteractiveText *> *words) {

  std::shuffle(words->begin(),
               words->end(),
               std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())
  );

}

std::vector<InteractiveText *> *GameHelper::GetUnifiedVector(std::vector<InteractiveText *> *a,
                                                             std::vector<InteractiveText *> *b) {

  std::vector<InteractiveText *> *all_words = new std::vector<InteractiveText *>();
  all_words->insert(all_words->end(), a->begin(), a->end());
  all_words->insert(all_words->end(), b->begin(), b->end());
  return all_words;

}

bool GameHelper::AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> *all_words,
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
