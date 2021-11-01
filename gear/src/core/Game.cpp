#include "Game.h"

gear::Game* gear::Game::game;

gear::Game::Game(void) {
  should_close = false;
}

gear::Game *gear::Game::get_Instance(void) {
  if(game == nullptr)
    game = new Game();
  return game;
}

void gear::Game::destroy(void) {
  delete this;
}

bool gear::Game::should_Close() const {
  return should_close;
}

void gear::Game::close() {
  should_close = true;
}