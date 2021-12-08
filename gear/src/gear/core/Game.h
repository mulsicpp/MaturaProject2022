#pragma once

#include "core.h"
#include "Window.h"

#if defined(GEAR_DEBUG)
#define GEAR_ROOT_PATH "../../.."
#endif

#if !defined(GEAR_ROOT_PATH)
#define GEAR_ROOT_PATH "."
#endif

_GEAR_START

class Game
{
private:
  static Game *game;
  gear::Window *main_Window;
  const char m_Path_To_App[512]{0};

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