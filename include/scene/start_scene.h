#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
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
  SDL_Color title_text_color_ = {0, 0, 0};

  Text *start_text_ = nullptr;
  TTF_Font *start_font_ = nullptr;
  SDL_Color start_text_color_ = {0, 0, 0};
  Button *start_button_ = nullptr;
  ButtonEvent start_button_event_ = NONE;
  const int start_button_width_ = 400;
  const int start_button_height_ = 100;
  const int start_font_size_ = 28;

  const double screen_height_;
  const double screen_width_;

};

}

#endif //CROSSLANGUAGEMATCH_SRC_SCENE_START_SCENE_H_
