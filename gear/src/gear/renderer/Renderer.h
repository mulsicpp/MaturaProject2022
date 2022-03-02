#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>

#include "Framebuffer.h"
#include "RenderPipeline.h"
#include <gear/scene/Scene.h>
#include <gear/collision/shapes/Shape.h>

#include "Camera.h"

_GEAR_START

class Renderer {
  friend class gear::Window;
private:
  static Framebuffer m_Framebuffer;

  static Camera *m_Camera;

  static GLFWwindow *m_Window;
  static int m_Window_Width, m_Window_Height;

public:
  static void create(int width, int height);
  static void destroy(void);

  static void set_Camera(Camera *camera);

  /*
  Enables or disables V-Sync.

  @param v_sync boolean to turn V-Sync on or off
  */
  static void set_V_Sync(bool v_sync);

  static void start_New_Frame(void);
  static void show_Frame(void);

  static void render_Scene(Scene *scene);
  static void render_Shape(const Shape *shape, const Vector<float, 4> &color);
  static void render_All_Hitboxes(Scene *scene);
};

_GEAR_END