#include "Game.h"

int main(void) {
  mete::Game game = mete::Game::get_Instance();

  game.on_Startup();

  while(!game.should_Close()) {
    game.in_Loop();
  }

  game.on_Shutdown();
}