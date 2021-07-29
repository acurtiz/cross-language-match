#include "game.h"

int main() {

  cross_language_match::Game game = cross_language_match::Game();
  game.LoadWordsViaFile("assets/txt/test-pairs-longer.csv");
  //game.LoadWordsViaString("dog,cat\nsky,ground\nred,blue");
  game.LoopDrawUntilQuit();
  return 0;

}
