#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "text/text.h"
#include "button/button.h"
#include "scene/scene.h"

#ifndef CROSSLANGUAGEMATCH_SRC_SCENE_LOAD_SCENE_EMSCRIPTEN_H_
#define CROSSLANGUAGEMATCH_SRC_SCENE_LOAD_SCENE_EMSCRIPTEN_H_

namespace cross_language_match {

static const char *kEmscriptenInputFilePath = "/input_file";

class LoadSceneEmscripten : public Scene {

 public:
  LoadSceneEmscripten(SDL_Renderer *renderer,
                      SDL_Window *window,
                      bool &global_quit,
                      int screen_height,
                      int screen_width
  );
  ~LoadSceneEmscripten();
  void RunPreLoop() override;
  void RunPostLoop() override;
  void RunSingleIterationEventHandler(SDL_Event &event) override;
  void RunSingleIterationLoopBody() override;

 private:

  Text *explanation_text_ = nullptr;
  Text *error_text_ = nullptr;

  TTF_Font *small_font_ = nullptr;
  const int small_font_size_ = 22;
  SDL_Color small_font_color_ = {0, 0, 0};

  Text *load_text_ = nullptr;
  Button *load_button_ = nullptr;
  ButtonEvent load_button_event_ = NONE;

  Text *return_button_text_ = nullptr;
  Button *return_button_ = nullptr;
  ButtonEvent return_button_event_ = NONE;

  TTF_Font *button_font_ = nullptr;
  SDL_Color button_text_color_ = {0, 0, 0};
  const int load_button_width_ = 400;
  const int load_button_height_ = 100;
  const int load_button_font_size_ = 28;
  const int return_button_width_ = 200;
  const int return_button_height_ = 100;

  const int screen_height_;
  const int screen_width_;

};

}

#endif //CROSSLANGUAGEMATCH_SRC_SCENE_LOAD_SCENE_EMSCRIPTEN_H_
