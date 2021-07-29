#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include <string>

#ifndef CROSSLANGUAGEMATCH_INCLUDE_TEXT_H_
#define CROSSLANGUAGEMATCH_INCLUDE_TEXT_H_

namespace cross_language_match {

class Text {
 public:
  Text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, std::string text, int wrap_length_pixels = -1);
  ~Text();
  void Free();
  void Render(int x, int y);
  int GetWidth() const;
  int GetHeight() const;
  std::string GetString() const;
 private:
  std::string text_string_;
  SDL_Texture *texture_;
  SDL_Renderer *renderer_;
  int width_;
  int height_;
};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_TEXT_H_
