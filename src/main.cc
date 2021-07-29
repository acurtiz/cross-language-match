#include "game.h"

int main() {

  cross_language_match::Game game = cross_language_match::Game();
  //game.LoadWordsViaFile("assets/txt/test-pairs.csv");
  game.LoadWordsViaString("alex,emily\ndog,cat\nsky,ground\nred,blue");
  game.LoopDrawUntilQuit();
  return 0;

}
