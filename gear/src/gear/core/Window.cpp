#include "Window.h"

gear::Window::Window() {}

gear::Window *gear::Window::create_Window(const char *name, int width, int height)
{
    gear::Window *ret = new gear::Window();
    ret->p_Window = glfwCreateWindow(width, height, name, NULL, NULL);
    return ret;
}

gear::Window *gear::Window::create_Window(const char *name, int x, int y, int width, int height)
{
    gear::Window *ret = new gear::Window();
    ret->p_Window = glfwCreateWindow(width, height, name, NULL, NULL);
    glfwSetWindowPos(ret->p_Window, x, y);
    return ret;
}

gear::Window *gear::Window::create_Fullscreen_Window(const char *name)
{
    gear::Window *ret = new gear::Window();
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int window_width = mode->width;
    int window_height = mode->height;
    ret->p_Window = glfwCreateWindow(window_width, window_height, name, glfwGetPrimaryMonitor(), NULL);
    return ret;
}

void gear::Window::destroy(void)
{
    glfwDestroyWindow(p_Window);
}

bool gear::Window::should_Close(void)
{
    return glfwWindowShouldClose(p_Window);
}

void gear::Window::set_Fullscreen()
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(p_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
}
void gear::Window::set_Windowed(int width, int height)
{
    glfwSetWindowMonitor(p_Window, nullptr, 0, 0, width, height, 0);
}

void gear::Window::set_Bounds(int x, int y, int width, int height)
{
    glfwSetWindowSize(p_Window, width, height);
    glfwSetWindowPos(p_Window, x, y);
}
void gear::Window::set_Size(int width, int height)
{
    glfwSetWindowSize(p_Window, width, height);
}
void gear::Window::set_Position(int x, int y)
{
    glfwSetWindowPos(p_Window, x, y);
}


void gear::Window::set_V_Sync(bool v_sync)
{
  glfwSwapInterval(v_sync ? 1 : 0);
}

void gear::Window::swap_Buffers(void)
{
    glfwSwapBuffers(p_Window);
}
void gear::Window::poll_Events(void)
{
    glfwPollEvents();
}