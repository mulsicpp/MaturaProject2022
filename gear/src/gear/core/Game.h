#pragma once

#include "core.h"
#include "Window.h"

_GEAR_START

class Game
{
private:
  static Game *game;
  gear::Window *main_Window;
  const char path_To_App[512]{0};
  const char *relative_Path_To_Assets = "..\\..";

  Game(void);

public:
  static Game *get_Instance(void);
  void run(void);

  void on_Startup(void);
  void on_Shutdown(void);
  void per_Frame(void);

  void close(int exit_code);
};

_GEAR_END