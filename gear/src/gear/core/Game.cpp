#include "Game.h"
#include <stdlib.h>
#include <stdio.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void gear_Init(void);

void gear_Terminate(void);

gear::Game* gear::Game::game;

gear::Game::Game(void) {}

gear::Game *gear::Game::get_Instance(void) {
  if(game == nullptr)
    game = new Game();
  return game;
}

void gear::Game::run(void) {
  gear_Init();
  on_Startup();
  while(1){
    per_Frame();
  }
}

void gear::Game::close(int exit_code) {
  on_Shutdown();
  gear_Terminate();
  delete game;
  exit(exit_code);
}

void gear_Init(void) {
  if(glfwInit() != GLFW_TRUE){
    printf("GLFW initialisation failed!\n");
    gear::Game::get_Instance()->close(1);
  }
}

void gear_Terminate(void) {
  glfwTerminate();
}