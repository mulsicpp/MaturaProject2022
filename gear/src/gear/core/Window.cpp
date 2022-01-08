#include "Window.h"
#include <gear/renderer/Renderer.h>
#include <glad/glad.h>

gear::Window::Window() {}

gear::Window *gear::Window::create_Window(const char *name, int width, int height)
{
  gear::Window *ret = new gear::Window();
  if (width <= 0)
    error("Width of a window cannot be less than or equal to 0");
  if (height <= 0)
    error("Height of a window cannot be less than or equal to 0");
  ret->m_Window = glfwCreateWindow(width, height, name, NULL, Renderer::m_Main_OpenGL_Context);
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
  ret->m_Window = glfwCreateWindow(width, height, name, NULL, Renderer::m_Main_OpenGL_Context);
  glfwSetWindowPos(ret->m_Window, x, y);
  return ret;
}

gear::Window *gear::Window::create_Fullscreen_Window(const char *name)
{
  gear::Window *ret = new gear::Window();
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  int window_width = mode->width;
  int window_height = mode->height;
  ret->m_Window = glfwCreateWindow(window_width, window_height, name, glfwGetPrimaryMonitor(), Renderer::m_Main_OpenGL_Context);
  return ret;
}

void gear::Window::destroy(void)
{
  if (m_FramebufferID != 0)
    glDeleteFramebuffers(1, &m_FramebufferID);
  if (m_TextureID != 0)
    glDeleteTextures(1, &m_TextureID);
  if (m_DepthbufferID != 0)
    glDeleteTextures(1, &m_DepthbufferID);
  if (m_VertexarrayID != 0)
    glDeleteVertexArrays(1, &m_VertexarrayID);
  glfwDestroyWindow(m_Window);
}

bool gear::Window::should_Close(void)
{
  return glfwWindowShouldClose(m_Window);
}

void GLAPIENTRY openGL_Message_Callback(unsigned int source,
                                        unsigned int type,
                                        unsigned int id,
                                        unsigned int severity,
                                        int length,
                                        const char *message,
                                        const void *userParam)
{
  if(type == GL_DEBUG_TYPE_ERROR)
    gear::error("OpenGL error!!!\nsource: %i\nid: %i\nseverity: %i\nmessage: \"%s\"", source, id, severity, message);
}

void gear::Window::make_Renderable(uint16_t width, uint16_t height)
{
  glfwMakeContextCurrent(m_Window);

#if defined(GEAR_DEBUG)
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(openGL_Message_Callback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

  glClearColor(0, 0, 0, 1);

  glGenVertexArrays(1, &m_VertexarrayID);
  glBindVertexArray(m_VertexarrayID);

  glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(0, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)));
  glVertexAttribBinding(1, 0);
  glEnableVertexAttribArray(1);

  glGenTextures(1, &m_TextureID);
  glBindTexture(GL_TEXTURE_2D, m_TextureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGenFramebuffers(1, &m_FramebufferID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

  glGenRenderbuffers(1, &m_DepthbufferID);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthbufferID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthbufferID);

  unsigned int draw_Buffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, draw_Buffers);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    gear::error("Framebuffer couldn't be completed");

  glfwMakeContextCurrent(Renderer::m_Window == nullptr ? Renderer::m_Main_OpenGL_Context : Renderer::m_Window->m_Window);
}

void gear::Window::set_Visible(bool visible)
{
  if (visible)
    glfwShowWindow(m_Window);
  else
    glfwHideWindow(m_Window);
}

void gear::Window::set_Resizable(bool resizable)
{
  glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, resizable);
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

void gear::Window::set_Title(const char *title)
{
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

void gear::Window::swap_Buffers(void)
{
  glfwSwapBuffers(m_Window);
}

void gear::Window::poll_Events(void)
{
  glfwPollEvents();
}