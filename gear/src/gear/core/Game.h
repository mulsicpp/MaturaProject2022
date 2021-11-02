#pragma once

#include <gear/core/core.h>

_GEAR_START

class Game {
private:
  static Game* game;

  Game(void);
public:
  static Game* get_Instance(void);
  void run(void);

  void on_Startup(void);
  void on_Shutdown(void);
  void per_Frame(void);
  
  void close(int exit_code);
};

_GEAR_END