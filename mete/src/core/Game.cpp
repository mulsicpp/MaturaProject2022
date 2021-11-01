#include "Game.h"

mete::Game* mete::Game::instance;

mete::Game::Game(void) {

}

mete::Game *mete::Game::get_Instance() {
  if(instance == nullptr)
    instance = new Game();
  return instance;
}

bool mete::Game::should_Close() {
  return true;
}