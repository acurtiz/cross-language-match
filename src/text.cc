#include "../include/text.h"
#include <SDL2_ttf/SDL_ttf.h>
#include <string>
#include <fmt/format.h>

namespace cross_language_match {

Text::Text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, std::string text) {

  renderer_ = renderer;

  SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), color);
  if (textSurface == nullptr) {
    throw std::runtime_error(fmt::format("Unable to render text surface, error: {}\n", TTF_GetError()));
  }

  texture_ = SDL_CreateTextureFromSurface(renderer_, textSurface);
  if (texture_ == nullptr) {
    throw std::runtime_error(fmt::format("Unable to create texture from surface, error: {}\n", SDL_GetError()));
  }

  width_ = textSurface->w;
  height_ = textSurface->h;

  SDL_FreeSurface(textSurface);

}

Text::~Text() {
  Free();
}

void Text::Free() {

  if (texture_ == nullptr) {
    return;
  }
  SDL_DestroyTexture(texture_);
  texture_ = nullptr;

}

void Text::Render(int x,
                  int y) {

  SDL_Rect dest_rect = {x, y, width_, height_};
  SDL_RenderCopy(renderer_, texture_, nullptr, &dest_rect);

}

int Text::GetWidth() const {
  return width_;
}

int Text::GetHeight() const {
  return height_;
}

}

