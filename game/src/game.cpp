#include <mete/mete.h>
#include <stdio.h>

void mete::Game::on_Startup(void) {
  printf("init game\n");
}

void mete::Game::in_Loop(void) {
  printf("hello from loop! %p %i %p %i\n", this, this->should_close, game, game->should_close);
  game->close();
}

void mete::Game::on_Shutdown(void) {
  printf("close game\n");
}