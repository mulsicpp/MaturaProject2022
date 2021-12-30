#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>
#include <gear/scene/Scene.h>

_GEAR_START

class Renderer {
  friend class gear::Window;
private:
  static GLFWwindow *main_OpenGL_Context;
public:
  static void create(void);
  static void destroy(void);
};

_GEAR_END