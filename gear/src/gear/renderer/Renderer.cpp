#include "Renderer.h"
#include <glad/glad.h>
#include <fstream>
#include <gear/core/debug/log.h>
#include <gear/data/FileStream.h>
#include <gear/scene/PositionComponent.h>
#include "SpriteComponent.h"

#include <gear/scene/Entity.h>

GLFWwindow *gear::Renderer::m_Main_OpenGL_Context = nullptr;
gear::RenderPipeline gear::Renderer::m_Upscale_PL;
gear::RenderPipeline gear::Renderer::m_Sprite_Nobatch_PL;

float gear::Renderer::m_Default_Vertexbuffer[16] = {
    -1.0, -1.0, 0.0, 0.0,
    1.0, -1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0,
    -1.0, 1.0, 0.0, 1.0};

unsigned int gear::Renderer::m_Default_Indexbuffer[6] = {0, 1, 2, 0, 2, 3};

gear::Window *gear::Renderer::m_Window = nullptr;

void gear::Renderer::create(void)
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(GEAR_DEBUG)
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  m_Main_OpenGL_Context = glfwCreateWindow(1, 1, "main_OpenGL_Context", NULL, NULL);
  glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
  glfwMakeContextCurrent(m_Main_OpenGL_Context);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    gear::error("Failed to load OpenGL");

#if defined(GEAR_DEBUG)
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(openGL_Message_Callback, nullptr);

  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
  GEAR_DEBUG_LOG("enabled debug callback");
#endif

  create_Upscale_PL();

  create_Sprite_Nobatch_PL();
}

void gear::Renderer::destroy(void)
{
  glDeleteBuffers(1, &m_Sprite_Nobatch_PL.m_Vertexbuffer);
  glDeleteBuffers(1, &m_Sprite_Nobatch_PL.m_Indexbuffer);
  glDeleteProgram(m_Sprite_Nobatch_PL.m_Shader);
  glDeleteBuffers(1, &m_Upscale_PL.m_Vertexbuffer);
  glDeleteBuffers(1, &m_Upscale_PL.m_Indexbuffer);
  glDeleteProgram(m_Upscale_PL.m_Shader);
  glfwDestroyWindow(m_Main_OpenGL_Context);
}

void gear::Renderer::set_Window(gear::Window *window)
{
  if (window != nullptr && window->m_DepthbufferID != 0 && window->m_TextureID != 0 && window->m_FramebufferID != 0 && window->m_Upscale_VertexarrayID != 0)
  {
    m_Window = window;
    glfwMakeContextCurrent(m_Window->m_Window);
    glUseProgram(m_Sprite_Nobatch_PL.m_Shader);
    glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Frame_Width"), m_Window->m_Framebuffer_Width);
    glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Frame_Height"), m_Window->m_Framebuffer_Height);
    m_Upscale_PL.m_Vertexarray = m_Window->m_Upscale_VertexarrayID;
  }
  else
    gear::error("Window is not renderable");
}

void gear::Renderer::set_V_Sync(bool v_sync)
{
  glfwSwapInterval(v_sync ? 1 : 0);
}

void gear::Renderer::start_New_Frame(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  // glBindFramebuffer(GL_FRAMEBUFFER, m_Window->m_FramebufferID);
  // glClear(GL_COLOR_BUFFER_BIT);
}

void gear::Renderer::show_Frame(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Window->m_TextureID);
  m_Upscale_PL.bind();

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  m_Window->swap_Buffers();
}

void gear::Renderer::setup_Test_Frame(void)
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Window->m_TextureID);

  uint32_t *data = new uint32_t[640 * 360];
  for (int i = 0; i < 640; i++)
    for (int j = 0; j < 360; j++)
      if (((i / 24) % 2) ^ ((j / 24) % 2) == 0)
      {
        data[i + j * 640] = 0xffffbf00;
      }
      else
      {
        data[i + j * 640] = 0xffc09000;
      }

  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 640, 360, GL_RGBA, GL_UNSIGNED_BYTE, data);

  delete[] data;

  glBindVertexArray(m_Upscale_PL.m_Vertexarray);
  glBindVertexBuffer(0, m_Upscale_PL.m_Vertexbuffer, 0, 4 * sizeof(float));
}

void gear::Renderer::render_Test_Frame(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Window->m_TextureID);
  m_Upscale_PL.bind();

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void sprite_Render_Callback(gear::PositionComponent &position, gear::SpriteComponent &sprite)
{
  GEAR_DEBUG_LOG("rendering sprite entity");
}

void gear::Renderer::render_Scene(gear::Scene *scene)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_Window->m_FramebufferID);
  m_Sprite_Nobatch_PL.bind();

  gear::Entity::for_Each(scene->get_ID(), sprite_Render_Callback);
}

void gear::Renderer::create_Upscale_PL(void)
{
  unsigned int vertex_Shader = load_Shader("shaders/vs_upscale.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = load_Shader("shaders/fs_upscale.glsl", GL_FRAGMENT_SHADER);

  m_Upscale_PL.m_Shader = link_Program(vertex_Shader, fragment_Shader);
  glUseProgram(m_Upscale_PL.m_Shader);
  glUniform1i(glGetUniformLocation(m_Upscale_PL.m_Shader, "u_Texture"), 0);
  GEAR_DEBUG_LOG("opengl program: %i", m_Upscale_PL.m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  glGenBuffers(1, &m_Upscale_PL.m_Vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_Upscale_PL.m_Vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), m_Default_Vertexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("vbo: %i", m_Upscale_PL.m_Vertexbuffer);

  glGenBuffers(1, &m_Upscale_PL.m_Indexbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Upscale_PL.m_Indexbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Default_Indexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("ibo: %i", m_Upscale_PL.m_Indexbuffer);
}

void gear::Renderer::create_Sprite_Nobatch_PL(void)
{
  unsigned int vertex_Shader = load_Shader("shaders/vs_sprite_nobatch.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = load_Shader("shaders/fs_sprite_nobatch.glsl", GL_FRAGMENT_SHADER);

  m_Sprite_Nobatch_PL.m_Shader = link_Program(vertex_Shader, fragment_Shader);
  glUseProgram(m_Sprite_Nobatch_PL.m_Shader);
  glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Texture"), 0);
  GEAR_DEBUG_LOG("opengl program: %i", m_Sprite_Nobatch_PL.m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  glGenBuffers(1, &m_Sprite_Nobatch_PL.m_Vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_Sprite_Nobatch_PL.m_Vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), m_Default_Vertexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("vbo: %i", m_Sprite_Nobatch_PL.m_Vertexbuffer);

  glGenBuffers(1, &m_Sprite_Nobatch_PL.m_Indexbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sprite_Nobatch_PL.m_Indexbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Default_Indexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("ibo: %i", m_Sprite_Nobatch_PL.m_Indexbuffer);
}



unsigned int gear::Renderer::load_Shader(const char *filename, unsigned int type)
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