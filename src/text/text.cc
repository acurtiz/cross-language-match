#include <string>
#include <boost/format.hpp>
#include <SDL2_ttf/SDL_ttf.h>
#include "text/text.h"

namespace cross_language_match {

Text::Text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, std::string text, int wrap_length_pixels) {

  renderer_ = renderer;
  text_string_ = text;

  SDL_Surface *text_surface;
  if (wrap_length_pixels == -1) {
    text_surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
  } else {
    text_surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, wrap_length_pixels);
  }
  if (text_surface == nullptr) {
    throw std::runtime_error(
        boost::str(boost::format("Unable to render text surface, error: %1\n") % TTF_GetError())
    );
  }

  texture_ = SDL_CreateTextureFromSurface(renderer_, text_surface);
  if (texture_ == nullptr) {
    throw std::runtime_error(
        boost::str(boost::format("Unable to create texture from surface, error: %1\n") % SDL_GetError())
    );
  }

  width_ = text_surface->w;
  height_ = text_surface->h;

  SDL_FreeSurface(text_surface);

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

void Text::Render(int x, int y) {

  SDL_Rect dest_rect = {x, y, width_, height_};
  SDL_RenderCopy(renderer_, texture_, nullptr, &dest_rect);

}

int Text::GetWidth() const {
  return width_;
}

int Text::GetHeight() const {
  return height_;
}

std::string Text::GetString() const {
  return text_string_;
}

}

