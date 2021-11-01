#pragma once

#include <core/core.h>

_METE_START

class Game {
private:
  Game(void);
public:
  static Game get_Instance(void);

  void on_Startup(void);
  void on_Shutdown(void);
  void in_Loop(void);

  bool should_Close();
};

_METE_END