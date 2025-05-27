#include "GameEngine.h"
#include "ProfilerTimer.h"

int main () {
  PROFILE_FUNCTION();

  GameEngine game("assets/levels/I.txt");

  game.run();
}