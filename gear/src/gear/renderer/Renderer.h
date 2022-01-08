#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>
#include <gear/scene/Scene.h>

#if defined(GEAR_DEBUG)

#define GL_CALL(x)                                                             \
  clear_GL_Errors();                                                           \
  x;                                                                           \
  log_GL_Errors(#x, __FILE__, __LINE__)

void clear_GL_Errors(void);

bool log_GL_Errors(const char *function, const char *file, int line);

#else
#define GL_CALL(x) x
#endif

_GEAR_START

class Renderer {
  friend class gear::Window;
private:
  static GLFWwindow *m_Main_OpenGL_Context;
  static unsigned int m_Sprite_Nobatch_Shader;
  static unsigned int m_Upscale_Shader;
  static unsigned int m_Upscale_VertexbufferID;
  static unsigned int m_Upscale_IndexbufferID;
  static Window *m_Window;

  static unsigned int create_Shader(const char *filename, unsigned int type);
  static unsigned int link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader);

public:
  static void create(void);
  static void destroy(void);

  static void set_Window(Window *window);

  static void clear_Frame(void);
  static void show_Frame(void);

  static void setup_Test_Frame(void);
  static void render_Test_Frame(void);
};

_GEAR_END