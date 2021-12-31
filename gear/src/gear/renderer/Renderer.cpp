#include "Renderer.h"
#include <glad/glad.h>
#include <fstream>
#include <gear/core/debug/log.h>

GLFWwindow *gear::Renderer::m_Main_OpenGL_Context = nullptr;
unsigned int gear::Renderer::m_Sprite_Nobatch_Shader = 0;

void gear::Renderer::create(void)
{
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  m_Main_OpenGL_Context = glfwCreateWindow(1, 1, "main_OpenGL_Context", NULL, NULL);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwMakeContextCurrent(m_Main_OpenGL_Context);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    gear::error("Failed to load OpenGL");

  unsigned int vertex_Shader = create_Shader("shaders/vs_sprite_nobatch.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = create_Shader("shaders/fs_sprite_nobatch.glsl", GL_FRAGMENT_SHADER);

  m_Sprite_Nobatch_Shader = link_Program(vertex_Shader, fragment_Shader);
  GEAR_DEBUG_LOG("opengl program: %i", m_Sprite_Nobatch_Shader);
}

void gear::Renderer::destroy(void)
{
  glfwDestroyWindow(m_Main_OpenGL_Context);
}



unsigned int gear::Renderer::create_Shader(const char *filename, unsigned int type)
{
  std::ifstream in = std::ifstream(filename);
  std::string shader_Source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

  unsigned int id = glCreateShader(type);
  const char *src = shader_Source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int res = 0;

  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  if (res == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    gear::error("Shader compilation failed: %s", message);
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int gear::Renderer::link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader)
{
  unsigned int program = glCreateProgram();

  glAttachShader(program, vertex_Shader);
  glAttachShader(program, fragment_Shader);
  glLinkProgram(program);
  glValidateProgram(program);

  int res;

  glGetProgramiv(program, GL_VALIDATE_STATUS, &res);
  if (res == GL_FALSE)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetProgramInfoLog(program, length, &length, message);
    gear::error("OpenGl program validation failed: %s", message);
    glDeleteShader(program);
    return 0;
  }

  return program;
}