#include "Game.h"

mete::Game* mete::Game::game;

mete::Game::Game(void) {
  should_close = false;
}

mete::Game *mete::Game::get_Instance(void) {
  if(game == nullptr)
    game = new Game();
  return game;
}

void mete::Game::destroy(void) {
  delete this;
}

bool mete::Game::should_Close() const {
  return should_close;
}

void mete::Game::close() {
  should_close = true;
}