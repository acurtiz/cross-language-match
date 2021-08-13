#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "shape/rectangle.h"

#ifndef CROSSLANGUAGEMATCH_INCLUDE_TEXT_H_
#define CROSSLANGUAGEMATCH_INCLUDE_TEXT_H_

namespace cross_language_match {

class Text : public Rectangle {
 public:
  Text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, std::string text, int wrap_length_pixels = -1);
  ~Text();
  void Free();
  void SetWidth(int width) override;
  void SetHeight(int height) override;
  void SetColor(SDL_Color color) override;
  void Render() override;
  std::string GetString() const;
 private:
  std::string text_string_;
  SDL_Texture *texture_;
  SDL_Renderer *renderer_;
};

}
#endif //CROSSLANGUAGEMATCH_INCLUDE_TEXT_H_
