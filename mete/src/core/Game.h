#pragma once

#include <core/core.h>

_METE_START

class Game {
private:
  static Game* game;
  bool should_close;

  Game(void);
public:
  static Game* get_Instance(void);
  void destroy(void);

  void on_Startup(void);
  void on_Shutdown(void);
  void in_Loop(void);

  bool should_Close() const;
  void close();
};

_METE_END