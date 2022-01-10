#pragma once

#include "core.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void GLAPIENTRY openGL_Message_Callback(unsigned int source,
                                        unsigned int type,
                                        unsigned int id,
                                        unsigned int severity,
                                        int length,
                                        const char *message,
                                        const void *userParam);

_GEAR_START

class Game;

class Renderer;

class Window
{
  friend class gear::Renderer;

private:
  GLFWwindow *m_Window = nullptr;
  unsigned int m_FramebufferID = 0;
  unsigned int m_TextureID = 0;
  unsigned int m_DepthbufferID = 0;
  unsigned int m_Upscale_VertexarrayID = 0;

  unsigned int m_Framebuffer_Width = 0, m_Framebuffer_Height = 0;

  /*
  Default Constructor
  */
  Window(void);

public:
  /*
  Creates a Window
  @param name name of the Window
  @param width The desired width, in screen coordinates, of the window. This must be greater than zero.
  @param height The desired height, in screen coordinates, of the window. This must be greater than zero.
  */
  static Window *create_Window(const char *name, int width, int height);

  /*
  Creates a Window
  @param name name of the Window
  @param x The x-coordinate of the upper-left corner of the content area.
  @param y The y-coordinate of the upper-left corner of the content area.
  @param width The desired width, in screen coordinates, of the window. This must be greater than zero.
  @param height The desired height, in screen coordinates, of the window. This must be greater than zero.
  */
  static Window *create_Window(const char *name, int x, int y, int width, int height);

  /*
  Creates a full screen Window
  @param name name of the Window
  */
  static Window *create_Fullscreen_Window(const char *name);

  /*
  Destroys the window and its context.
  */
  void destroy(void);

  /*
  This function returns the value of the close flag of the window.
  */
  bool should_Close(void);

  /*
  Makes the window renderable.
  @param width the width of the backbuffer
  @param height the height of the backbuffer
  */
  void make_Renderable(uint16_t width, uint16_t height);

  /*
  Make the window visible or invisible.
  @param visible visibility of window
  */
  void set_Visible(bool visible);

  /*
  Make the window resizable or not resizable.
  @param resizable sets window resizable if true, if false non resizable
  */
  void set_Resizable(bool resizable);

  /*
  Sets the window full screen.
  */
  void set_Fullscreen();

  /*
  Sets the window to windowed mode
  @param width The desired width, in screen coordinates, of the window. This must be greater than zero.
  @param height The desired height, in screen coordinates, of the window. This must be greater than zero.
  */
  void set_Windowed(int width, int height);

  /*
  Sets the title of the window.
  @param title title of window
  */
  void set_Title(const char *title);

  /*
  Sets the size and position of the window. For full screen windows, this function updates the resolution of its desired video mode and switches to the video mode closest to it, without affecting the window's context.
  @param x The x-coordinate of the upper-left corner of the content area.
  @param y The y-coordinate of the upper-left corner of the content area.
  @param width The desired width, in screen coordinates, of the window. This must be greater than zero.
  @param height The desired height, in screen coordinates, of the window. This must be greater than zero.
  */
  void set_Bounds(int x, int y, int width, int height);

  /*
  Sets the size of the window
  @param width The desired width, in screen coordinates, of the window. This must be greater than zero.
  @param height The desired height, in screen coordinates, of the window. This must be greater than zero.
  */
  void set_Size(int width, int height);

  /*
  Sets the position of the window. If the window is a full screen window, this function does nothing.
  @param x The x-coordinate of the upper-left corner of the content area.
  @param y The y-coordinate of the upper-left corner of the content area.
  */
  void set_Position(int x, int y);

  /*
  Swaps the front and back buffers of the specified window.
  */
  void swap_Buffers(void);

  /*
  Processes all pending events. This function processes only those events that are already in the event queue.
  */
  void poll_Events(void);
};

_GEAR_END