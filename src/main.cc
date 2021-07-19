#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <text.h>
#include <fmt/core.h>

namespace cross_language_match {

void Init();
void Close();
void Draw();

SDL_Window *kWindow = nullptr;
SDL_Renderer *kRenderer = nullptr;
TTF_Font *kFont = nullptr;

void Init() {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw std::runtime_error(fmt::format("SDL could not initialize, error: {}\n", SDL_GetError()));
  }

  kWindow = SDL_CreateWindow("Cross Language Match",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             1080,
                             800,
                             SDL_WINDOW_SHOWN);

  if (kWindow == nullptr) {
    throw std::runtime_error(fmt::format("Window could not be created, error: {}\n", SDL_GetError()));
  }

  kRenderer = SDL_CreateRenderer(kWindow, -1, SDL_RENDERER_ACCELERATED);

  if (kRenderer == nullptr) {
    throw std::runtime_error(fmt::format("Renderer could not be created, error: {}\n", SDL_GetError()));
  }
  SDL_SetRenderDrawColor(kRenderer, 0x00, 0x00, 0x00, 0xFF);

  if (TTF_Init() == -1) {
    throw std::runtime_error(fmt::format("SDL_ttf could not be initialized, error: {}\n", TTF_GetError()));
  }

  kFont = TTF_OpenFont("assets/fonts/OpenSans-Regular.ttf", 28);

  if (kFont == NULL) {
    throw std::runtime_error(fmt::format("Failed to load font, error: {}\n", TTF_GetError()));
  }

}

void Close() {

  SDL_DestroyWindow(kWindow);
  kWindow = nullptr;
  TTF_CloseFont(kFont);
  kFont = nullptr;
  TTF_Quit();
  SDL_Quit();

}

void Draw() {

  SDL_SetRenderDrawColor(kRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(kRenderer);
  SDL_RenderPresent(kRenderer);

  SDL_Event e;
  bool quit = false;

  Text *text = new Text(kRenderer, kFont, {0x00, 0x00, 0x00}, "Hello World!");

  while (!quit) {

    while (SDL_PollEvent(&e)) {

      if (e.type == SDL_QUIT) {
        quit = true;
      }

    }
    SDL_RenderClear(kRenderer);

    text->Render(100, 100);

    SDL_RenderPresent(kRenderer);

  }

}

}

int main() {
  cross_language_match::Init();
  cross_language_match::Draw();
  cross_language_match::Close();
  return 0;
}
