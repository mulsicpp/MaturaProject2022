#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>

#include "Framebuffer.h"
#include "RenderPipeline.h"
#include <gear/scene/Scene.h>
#include <gear/collision/shapes/Shape.h>

#include <gear/math/Vector.h>

#include "Camera.h"

_GEAR_START

class Renderer {
  friend class gear::Window;
private:
  static Framebuffer m_Framebuffer;

  static Camera *m_Camera;

  static GLFWwindow *m_Window;

  static Vector<int, 2> m_Top_Left, m_Bottom_Right;

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

  static void set_Viewport(Vector<int, 2> top_Left, Vector<int, 2> bottom_Right);

  static void render_Scene(Scene *scene);
  static void render_Shape(const Shape *shape, const Vector<float, 4> &color);
  static void render_All_Hitboxes(Scene *scene);
};

_GEAR_END