#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>
#include <gear/scene/Scene.h>

#include "RenderPipeline.h"

_GEAR_START

class Renderer {
  friend class gear::Window;
private:
  static GLFWwindow *m_Main_OpenGL_Context;
  static RenderPipeline m_Upscale_PL;
  static RenderPipeline m_Sprite_Nobatch_PL;
  static float m_Default_Vertexbuffer[16];
  static unsigned int m_Default_Indexbuffer[6];
  static Window *m_Window;

  static unsigned int load_Shader(const char *filename, unsigned int type);
  static unsigned int link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader);

  static void create_Upscale_PL(void);
  static void create_Sprite_Nobatch_PL(void);

public:
  static void create(void);
  static void destroy(void);

  static void set_Window(Window *window);

  /*
  Enables or disables V-Sync.

  @param v_sync boolean to turn V-Sync on or off
  */
  static void set_V_Sync(bool v_sync);

  static void start_New_Frame(void);
  static void show_Frame(void);

  static void setup_Test_Frame(void);
  static void render_Test_Frame(void);

  static void render_Scene(Scene *scene);
};

_GEAR_END