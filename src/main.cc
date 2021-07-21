#include "game.h"

int main() {

  cross_language_match::Game game = cross_language_match::Game();
  game.LoadWordPairs("assets/txt/test-pairs-longer.csv");
  game.LoopDrawUntilQuit();
  return 0;

}
