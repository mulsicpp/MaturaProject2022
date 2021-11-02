#include <gear/gear.h>
#include <stdio.h>

void gear::Game::on_Startup(void) {
  printf("init game\n");
}

void gear::Game::per_Frame(void) {
  printf("hello\n");
  game->close(0);
}

void gear::Game::on_Shutdown(void) {
  printf("close game\n");
}