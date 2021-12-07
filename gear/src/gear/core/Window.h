#pragma once

#include "core.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

_GEAR_START

class Window
{
private:
  GLFWwindow *m_Window;

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
  Enables or disables V-Sync for the window.

  @param v_sync boolean to turn V-Sync on or off
  */
  static void set_V_Sync(bool v_sync);

  /*
  Swaps the front and back buffers of the specified window.
  */
  void swap_Buffers(void);

  /*
  Processes all pending events. This function processes only those events that are already in the event queue.
  */
  void poll_Events(void);

  /*
  Sets the window as the current render context.
  */
  void make_Render_Context_Current(void);
};

_GEAR_END