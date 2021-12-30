#include "Renderer.h"
#include <glad/glad.h>

GLFWwindow *gear::Renderer::main_OpenGL_Context = nullptr;

void gear::Renderer::create(void)
{
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  main_OpenGL_Context = glfwCreateWindow(1, 1, "main_OpenGL_Context", NULL, NULL);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwMakeContextCurrent(main_OpenGL_Context);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    gear::error("Failed to load OpenGL");
}

void gear::Renderer::destroy(void)
{
  glfwDestroyWindow(main_OpenGL_Context);
}
