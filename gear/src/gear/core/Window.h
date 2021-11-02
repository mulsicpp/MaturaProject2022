#pragma once

#include "core.h"
#include "Game.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

_GEAR_START

class Window {
private:
  GLFWwindow *p_Window;

  Window(void);
public:
  static Window *create_Window(const char *name, int width, int height);
  static Window *create_Window(const char *name, int x, int y, int width, int height);
  static Window *create_Fullscreen_Window(const char *name);

  void destroy(void);

  void set_Fullscreen();
  void set_Windowed(int width, int height);

  void set_Bounds(int x, int y, int width, int height);
  void set_Size(int width, int height);
  void set_Position(int x, int y);

  void swap_Buffers(void);
  void poll_Events(void);
};

_GEAR_END