#include "game.h"
#include <stdio.h>
#include "scene/load_scene_emscripten.h"

// Emscripten exporting only works for C functions
extern "C" {

int main() {

  cross_language_match::Game game = cross_language_match::Game();
  game.Run();
  return 0;

}

// This function is intended to be exported with Emscripten so it can be invoked via Javascript to
// persist a web-loaded file in the local virtual filesystem (so it can be accessed in the game).
void persist_file(char *contents_to_persist) {
  FILE *file = fopen(cross_language_match::kEmscriptenInputFilePath, "w+");
  fprintf(file, "%s", contents_to_persist);
  fclose(file);
}

}

