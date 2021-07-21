#include "game.h"

int main() {

  cross_language_match::Game game = cross_language_match::Game();
  game.LoadWords("assets/txt/test-pairs.csv");
  game.LoopDrawUntilQuit();
  return 0;

}
