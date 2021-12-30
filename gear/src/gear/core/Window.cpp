#include "Window.h"
#include <gear/renderer/Renderer.h>

gear::Window::Window() {}

gear::Window *gear::Window::create_Window(const char *name, int width, int height)
{
  gear::Window *ret = new gear::Window();
  if (width <= 0)
    error("Width of a window cannot be less than or equal to 0");
  if (height <= 0)
    error("Height of a window cannot be less than or equal to 0");
  ret->m_Window = glfwCreateWindow(width, height, name, NULL, Renderer::main_OpenGL_Context);
  if (ret->m_Window == nullptr)
  {
    const char *message;
    glfwGetError(&message);
    gear::error("%s\n", message);
  }
  return ret;
}

gear::Window *gear::Window::create_Window(const char *name, int x, int y, int width, int height)
{
  gear::Window *ret = new gear::Window();
  if (width <= 0)
    error("Width of a window cannot be less than or equal to 0");
  if (height <= 0)
    error("Height of a window cannot be less than or equal to 0");
  ret->m_Window = glfwCreateWindow(width, height, name, NULL, Renderer::main_OpenGL_Context);
  glfwSetWindowPos(ret->m_Window, x, y);
  return ret;
}

gear::Window *gear::Window::create_Fullscreen_Window(const char *name)
{
  gear::Window *ret = new gear::Window();
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int window_width = mode->width;
  int window_height = mode->height;
  ret->m_Window = glfwCreateWindow(window_width, window_height, name, glfwGetPrimaryMonitor(), Renderer::main_OpenGL_Context);
  return ret;
}

void gear::Window::destroy(void)
{
  glfwDestroyWindow(m_Window);
}

bool gear::Window::should_Close(void)
{
  return glfwWindowShouldClose(m_Window);
}

void gear::Window::set_Visible(bool visible)
{
  if (visible)
    glfwShowWindow(m_Window);
  else
    glfwHideWindow(m_Window);
}

void gear::Window::set_Fullscreen()
{
  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *mode = glfwGetVideoMode(monitor);
  glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}
void gear::Window::set_Windowed(int width, int height)
{
  glfwSetWindowMonitor(m_Window, nullptr, 0, 0, width, height, 0);
}

void gear::Window::set_Title(const char *title) {
  glfwSetWindowTitle(m_Window, title);
}

void gear::Window::set_Bounds(int x, int y, int width, int height)
{
  if (width <= 0)
    error("Width of a window cannot be less than or equal to 0");
  if (height <= 0)
    error("Height of a window cannot be less than or equal to 0");
  glfwSetWindowSize(m_Window, width, height);
  glfwSetWindowPos(m_Window, x, y);
}
void gear::Window::set_Size(int width, int height)
{
  if (width <= 0)
    error("Width of a window cannot be less than or equal to 0");
  if (height <= 0)
    error("Height of a window cannot be less than or equal to 0");
  glfwSetWindowSize(m_Window, width, height);
}
void gear::Window::set_Position(int x, int y)
{
  glfwSetWindowPos(m_Window, x, y);
}

void gear::Window::set_V_Sync(bool v_sync)
{
  glfwSwapInterval(v_sync ? 1 : 0);
}

void gear::Window::swap_Buffers(void)
{
  glfwSwapBuffers(m_Window);
}

void gear::Window::poll_Events(void)
{
  glfwPollEvents();
}

void gear::Window::make_Render_Context_Current(void)
{
  glfwMakeContextCurrent(m_Window);
}