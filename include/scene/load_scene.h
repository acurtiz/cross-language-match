#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "word_loader/file_word_loader.h"
#include "text/text.h"
#include "button/button.h"
#include "scene/scene.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_SCENE_LOAD_SCENE_H_
#define CROSSLANGUAGEMATCH_INCLUDE_SCENE_LOAD_SCENE_H_

namespace cross_language_match {

static const char *kEmscriptenInputFilePath = "/input_file";

class LoadScene : public Scene {

 public:
  LoadScene(SDL_Renderer *renderer,
            SDL_Window *window,
            bool &global_quit,
            int screen_height,
            int screen_width
  );
  ~LoadScene();
  void RunPreLoop() override;
  void RunPostLoop() override;
  void RunSingleIterationEventHandler(SDL_Event &event) override;
  void RunSingleIterationLoopBody() override;

 private:

  void HandleBeginEvent(SDL_Event &event);
  void SetErrorMessage(std::string error_message);
  void ClearErrorMessage();
  bool IsFileLoaded();
  bool IsFileReadyForGame();
  void AllocateLoadedFileName();

  Text *explanation_text_ = nullptr;
  Text *error_text_ = nullptr;

  TTF_Font *small_font_ = nullptr;
  const int small_font_size_ = 22;
  SDL_Color small_font_color_ = {0, 0, 0};

  Text *load_text_ = nullptr;
  Button *load_button_ = nullptr;
  ButtonEvent load_button_event_ = NONE;

  Text *begin_text_ = nullptr;
  Button *begin_button_ = nullptr;
  ButtonEvent begin_button_event_ = NONE;

  Text *return_button_text_ = nullptr;
  Button *return_button_ = nullptr;
  ButtonEvent return_button_event_ = NONE;

  TTF_Font *button_font_ = nullptr;
  SDL_Color button_text_color_ = {0, 0, 0};

  const int wide_button_width_ = 400;
  const int wide_button_height_ = 100;
  const int wide_button_font_size_ = 28;

  const int return_button_width_ = 200;
  const int return_button_height_ = 100;

  const int screen_height_;
  const int screen_width_;

  char *loaded_file_name_ = nullptr;
  bool loaded_file_has_been_processed_ = false;
  FileWordLoader *word_loader_;

};

}

#endif //CROSSLANGUAGEMATCH_INCLUDE_SCENE_LOAD_SCENE_H_
