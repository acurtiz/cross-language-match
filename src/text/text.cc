#include <string>
#include <boost/format.hpp>
#include <SDL_ttf.h>
#include "text/text.h"

namespace cross_language_match {

Text::Text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, std::string text, int wrap_length_pixels)
    : Rectangle(renderer) {

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
        boost::str(boost::format("Unable to render text surface, error: %1%\n") % TTF_GetError())
    );
  }

  texture_ = SDL_CreateTextureFromSurface(renderer, text_surface);
  if (texture_ == nullptr) {
    throw std::runtime_error(
        boost::str(boost::format("Unable to create texture from surface, error: %1%\n") % SDL_GetError())
    );
  }

  Rectangle::SetWidth(text_surface->w);
  Rectangle::SetHeight(text_surface->h);

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

void Text::Render() {

  SDL_Rect dest_rect = {GetTopLeftX(), GetTopLeftY(), GetWidth(), GetHeight()};
  SDL_RenderCopy(renderer_, texture_, nullptr, &dest_rect);

}

std::string Text::GetString() const {
  return text_string_;
}

void Text::SetHeight(int height) {
  throw std::runtime_error("Mutating height on text object not supported; ignoring\n");
}

void Text::SetWidth(int width) {
  throw std::runtime_error("Mutating width on text object not supported; ignoring\n");
}

void Text::SetColor(SDL_Color color) {
  throw std::runtime_error("Mutating color on text object not supported; ignoring\n");
}

}

