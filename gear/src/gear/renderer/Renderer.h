#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>

#include "Framebuffer.h"
#include "RenderPipeline.h"
#include <gear/scene/Scene.h>

_GEAR_START

class Renderer {
  friend class gear::Window;
private:
  static Framebuffer m_Framebuffer;

  static GLFWwindow *m_Window;
  static int m_Window_Width, m_Window_Height;

public:
  static void create(int width, int height);
  static void destroy(void);

  static void set_Window(Window *window);

  /*
  Enables or disables V-Sync.

  @param v_sync boolean to turn V-Sync on or off
  */
  static void set_V_Sync(bool v_sync);

  static void start_New_Frame(void);
  static void show_Frame(void);

  static void render_Scene(Scene *scene);
};

_GEAR_END