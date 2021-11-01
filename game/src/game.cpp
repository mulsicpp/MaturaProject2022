#include <gear/gear.h>
#include <stdio.h>

void gear::Game::on_Startup(void) {
  printf("init game\n");
}

void gear::Game::in_Loop(void) {
  printf("hello from loop! %p %i %p %i\n", this, this->should_close, game, game->should_close);
  game->close();
}

void gear::Game::on_Shutdown(void) {
  printf("close game\n");
}