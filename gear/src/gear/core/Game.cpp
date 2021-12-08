#define GEAR_INCLUDE_WINDOWS_H
#include "Game.h"
#include "debug/log.h"
#include <filesystem>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


void gear_Init(void);

void gear_Terminate(void);

gear::Game *gear::Game::game = nullptr;

gear::Game::Game(void) {
#if defined(GEAR_PLATFORM_WINDOWS)
  char temp_Path[MAX_PATH];
  GetModuleFileNameA(NULL, temp_Path, MAX_PATH);
  std::filesystem::path path = std::filesystem::weakly_canonical(std::filesystem::path(temp_Path).parent_path());
  strcpy((char*)m_Path_To_App, path.string().c_str());

#elif defined(GEAR_PLATFORM_LINUX)
  char path[100] = "some/path";
  strcpy((char*)m_Path_To_App, path);
#endif
}

gear::Game *gear::Game::get_Instance(void) {
  if(game == nullptr)
    game = new Game();
  return game;
}

void gear::Game::run(void) {
  std::filesystem::current_path(m_Path_To_App);
  std::filesystem::current_path(GEAR_ROOT_PATH);
  gear_Init();
  on_Startup();
  while(1){
    per_Frame();
  }
}

void gear::Game::close(int exit_code) {
  on_Shutdown();
  gear_Terminate();
  delete this;
  exit(exit_code);
}

void gear_Init(void) {
  if(glfwInit() != GLFW_TRUE){
    GEAR_DEBUG_LOG("GLFW initialisation failed!\n");
    gear::Game::get_Instance()->close(1);
  }
}

void gear_Terminate(void) {
  glfwTerminate();
}