#include "Renderer.h"
#include <glad/glad.h>
#include <fstream>
#include <gear/core/debug/log.h>

GLFWwindow *gear::Renderer::m_Main_OpenGL_Context = nullptr;
unsigned int gear::Renderer::m_Sprite_Nobatch_Shader = 0;
unsigned int gear::Renderer::m_Upscale_Shader = 0;
unsigned int gear::Renderer::m_Upscale_VertexbufferID = 0;
unsigned int gear::Renderer::m_Upscale_IndexbufferID = 0;

gear::Window *gear::Renderer::m_Window = nullptr;

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

  vertex_Shader = create_Shader("shaders/vs_upscale.glsl", GL_VERTEX_SHADER);
  fragment_Shader = create_Shader("shaders/fs_upscale.glsl", GL_FRAGMENT_SHADER);

  m_Upscale_Shader = link_Program(vertex_Shader, fragment_Shader);
  GEAR_DEBUG_LOG("opengl program: %i", m_Upscale_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  float upscale_Vertex_Buffer[16] = {
    -1.0, -1.0, 0.0, 0.0,
     1.0, -1.0, 1.0, 0.0,
     1.0,  1.0, 1.0, 1.0,
    -1.0,  1.0, 0.0, 1.0
  };

  unsigned int upscale_Index_Buffer[6] = {0, 1, 2, 0, 2, 3};

  glGenBuffers(1, &m_Upscale_VertexbufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_Upscale_VertexbufferID);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), upscale_Vertex_Buffer, GL_STATIC_DRAW);

  glGenBuffers(1, &m_Upscale_IndexbufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Upscale_IndexbufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), upscale_Index_Buffer, GL_STATIC_DRAW);
}

void gear::Renderer::destroy(void)
{
  glDeleteBuffers(1, &m_Upscale_VertexbufferID);
  glDeleteBuffers(1, &m_Upscale_IndexbufferID);
  glDeleteProgram(m_Sprite_Nobatch_Shader);
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

void gear::Renderer::set_Window(gear::Window *window)
{
  if(window->m_DepthbufferID != 0 && window->m_TextureID != 0 && window->m_FramebufferID != 0 && window->m_VertexarrayID != 0)
  {
    m_Window = window;
    glfwMakeContextCurrent(m_Window->m_Window);
    glClearColor(0, 0, 0, 1);
  }
  else
    gear::error("Window is not renderable");
}

void gear::Renderer::clear_Frame(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void gear::Renderer::show_Frame(void)
{
  m_Window->swap_Buffers();
}