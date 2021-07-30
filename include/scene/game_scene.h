#include <string>
#include <map>
#include <SDL2_ttf/SDL_ttf.h>
#include <button/button.h>
#include "scene.h"
#include "text/interactive_text.h"

#ifndef CROSSLANGUAGEMATCH_SRC_GAME_SCENE_H_
#define CROSSLANGUAGEMATCH_SRC_GAME_SCENE_H_

namespace cross_language_match {

class GameScene : public Scene {

 public:
  GameScene(SDL_Renderer *renderer,
            SDL_Window *window,
            bool &global_quit,
            int screen_height,
            int screen_width,
            std::map<std::string, std::string> word_pairs);
  ~GameScene();

  void RunPreLoop() override;
  void RunPostLoop() override;
  void RunSingleIterationEventHandler(SDL_Event &event) override;
  void RunSingleIterationLoopBody() override;

 private:
  void PrepareCurrentWords();
  void CleanCurrentWords();
  bool AreAllWordsLinkedAndCorrect(std::vector<InteractiveText *> *all_words,
                                   std::map<std::string, std::string> *expected_word_pairs);
  void Shuffle(std::vector<InteractiveText *> *vector);
  std::vector<InteractiveText *> *GetUnifiedVector(std::vector<InteractiveText *> *a,
                                                   std::vector<InteractiveText *> *b);

  TTF_Font *font_ = nullptr;
  SDL_Color text_color_ = {0, 0, 0};

  Text *incorrect_text_ = nullptr;
  Text *correct_text_ = nullptr;

  Text *submit_text_ = nullptr;
  Button *submit_button_ = nullptr;
  Text *next_round_text_ = nullptr;
  Button *next_round_button_ = nullptr;
  Text *return_text_ = nullptr;
  Button *return_button_ = nullptr;

  ButtonEvent submit_button_event_ = NONE;
  ButtonEvent next_round_button_event_ = NONE;
  ButtonEvent return_button_event_ = NONE;

  std::vector<InteractiveText *> *left_words_ = nullptr;
  std::vector<InteractiveText *> *right_words_ = nullptr;
  std::vector<InteractiveText *> *left_and_right_words_ = nullptr;
  std::map<std::string, std::string> *remaining_word_pairs_ = nullptr;
  std::map<std::string, std::string> *current_word_pairs_ = nullptr;

  bool all_rounds_complete_ = false;
  bool current_round_is_complete_ = false;
  bool last_submission_was_incorrect_ = false;

  const int screen_height_;
  const int screen_width_;
  const double screen_height_percentage_reserved_for_words_ = 0.6;
  const int padding_word_columns_ = 100;
  const int font_size_ = 28;
  const int padding_individual_words_ = 15;
  const int submit_button_width_ = 200;
  const int submit_button_height_ = 100;

  const int next_button_width_ = 200;
  const int next_button_height_ = 100;

  const int return_button_width_ = 200;
  const int return_button_height_ = 100;

  // Words to be presented per round should be a function of the screen height
  const int words_to_present_per_round_ = (int) (screen_height_ * screen_height_percentage_reserved_for_words_)
      / (font_size_ + InteractiveText::GetPaddingPerSide() * 2 + padding_individual_words_);

};

}

#endif //CROSSLANGUAGEMATCH_SRC_GAME_SCENE_H_
