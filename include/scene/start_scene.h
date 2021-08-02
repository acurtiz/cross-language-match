#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "scene/scene.h"
#include "text/text.h"
#include "button/button.h"

#ifndef CROSSLANGUAGEMATCH_SRC_SCENE_START_SCENE_H_
#define CROSSLANGUAGEMATCH_SRC_SCENE_START_SCENE_H_

namespace cross_language_match {

class StartScene : public Scene {

 public:
  StartScene(SDL_Renderer *renderer, SDL_Window *window, bool &global_quit, int screen_height, int screen_width);
  ~StartScene();
  void RunPreLoop() override;
  void RunPostLoop() override;
  void RunSingleIterationEventHandler(SDL_Event &event) override;
  void RunSingleIterationLoopBody() override;

 private:

  TTF_Font *title_font_ = nullptr;
  Text *title_text_ = nullptr;
  const int title_font_size_ = 44;
  SDL_Color title_text_color_ = {0xFF, 0xFF, 0xFF};

  TTF_Font *button_font_ = nullptr;
  const int button_font_size_ = 28;

  Text *start_text_ = nullptr;
  SDL_Color start_text_color_ = {0, 0, 0};
  Button *start_button_ = nullptr;
  ButtonEvent start_button_event_ = NONE;
  const int start_button_width_ = 300;
  const int start_button_height_ = 50;

  Text *help_text_ = nullptr;
  SDL_Color help_text_color_ = {0, 0, 0};
  Button *help_button_ = nullptr;
  ButtonEvent help_button_event_ = NONE;
  const int help_button_width_ = 300;
  const int help_button_height_ = 50;

  const int screen_height_;
  const int screen_width_;

};

}

#endif //CROSSLANGUAGEMATCH_SRC_SCENE_START_SCENE_H_
