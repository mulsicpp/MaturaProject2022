#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>
#include <gear/scene/Scene.h>

#include "Framebuffer.h"
#include "RenderPipeline.h"

#include <gear/scene/PositionComponent.h>
#include "SpriteComponent.h"

_GEAR_START

class Renderer {
  friend class gear::Window;
private:
  static Framebuffer m_Framebuffer;
  static RenderPipeline m_Upscale_PL;
  static RenderPipeline m_Sprite_Nobatch_PL;

  static float m_Default_Vertexbuffer[16];
  static unsigned int m_Default_Indexbuffer[6];

  static float m_Sprite_Nobatch_Vertexbuffer_Data[16];

  static GLFWwindow *m_Window;
  static int m_Window_Width, m_Window_Height;

  static void sprite_Render_Callback(PositionComponent &position, SpriteComponent &sprite);

  static void create_Upscale_PL(void);
  static void create_Sprite_Nobatch_PL(void);

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

  static void setup_Test_Frame(void);
  static void render_Test_Frame(void);

  static void render_Scene(Scene *scene);
};

_GEAR_END