#include <mete/mete.h>
#include <stdio.h>

void mete::Game::on_Startup(void) {
  printf("init game\n");
}

void mete::Game::in_Loop(void) {
  
}

void mete::Game::on_Shutdown(void) {
  printf("close game\n");
}