#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "button/button.h"
#include "button/button_event.h"
#include "text/text.h"
#include "scene/scene.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_SCENE_HELP_SCENE_H_
#define CROSSLANGUAGEMATCH_INCLUDE_SCENE_HELP_SCENE_H_

namespace cross_language_match {

class HelpScene : public Scene {

 public:
  HelpScene(SDL_Renderer *renderer, SDL_Window *window, bool &global_quit, int screen_height, int screen_width);
  ~HelpScene();
  void RunPreLoop() override;
  void RunPostLoop() override;
  void RunSingleIterationEventHandler(SDL_Event &event) override;
  void RunSingleIterationLoopBody() override;

 private:

  TTF_Font *explanation_font_ = nullptr;
  Text *explanation_text_ = nullptr;
  const int explanation_font_size_ = 22;
  SDL_Color explanation_text_color_ = {0xFF, 0xFF, 0xFF};

  Text *return_text_ = nullptr;
  TTF_Font *return_button_font_ = nullptr;
  SDL_Color return_text_color_ = {0, 0, 0};

  Button *return_button_ = nullptr;
  ButtonEvent return_button_event_ = NONE;

  const int button_width_ = 400;
  const int button_height_ = 100;
  const int button_font_size_ = 28;

  const int screen_height_;
  const int screen_width_;

};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_SCENE_HELP_SCENE_H_
