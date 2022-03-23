#pragma once

#include "core.h"
#include "Window.h"

#if defined(GEAR_DEBUG)
#define GEAR_ROOT_PATH "../../../.."
#endif

#if !defined(GEAR_ROOT_PATH)
#define GEAR_ROOT_PATH "."
#endif

_GEAR_START

class Game
{
protected:
  gear::Window *m_Window = nullptr;
  const char m_Path_To_App[512]{0};
  gear::Window *main_Window;
  static double m_Delta_Time;

  void gear_Init(void);

  void gear_Terminate(void);

public:
  Game(void);
  void run(void);

  virtual void on_Startup(void) = 0;
  virtual void on_Shutdown(void) = 0;
  virtual void per_Frame(void) = 0;

  void close(int exit_code);

  const char *get_App_Path(void) const;

  static double get_Delta_Time(void);
};

_GEAR_END