#include "Game.h"
#include <stdlib.h>

void gearInit(void);

void gearTerminate(void);

gear::Game* gear::Game::game;

gear::Game::Game(void) {}

gear::Game *gear::Game::get_Instance(void) {
  if(game == nullptr)
    game = new Game();
  return game;
}

void gear::Game::run(void) {
  gearInit();
  on_Startup();
  while(1){
    per_Frame();
  }
}

void gear::Game::close(int exit_code) {
  on_Shutdown();
  gearTerminate();
  delete game;
  exit(exit_code);
}

void gearInit(void) {

}

void gearTerminate(void) {

}