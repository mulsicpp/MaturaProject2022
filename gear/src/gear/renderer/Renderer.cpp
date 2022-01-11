#include "Renderer.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>
#include <gear/data/FileStream.h>
#include <gear/scene/PositionComponent.h>
#include "SpriteComponent.h"

#include <gear/scene/Entity.h>

gear::Framebuffer gear::Renderer::m_Framebuffer;
gear::RenderPipeline gear::Renderer::m_Upscale_PL;
gear::RenderPipeline gear::Renderer::m_Sprite_Nobatch_PL;

float gear::Renderer::m_Default_Vertexbuffer[16] = {
    -1.0, -1.0, 0.0, 0.0,
    1.0, -1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0,
    -1.0, 1.0, 0.0, 1.0};

unsigned int gear::Renderer::m_Default_Indexbuffer[6] = {0, 1, 2, 0, 2, 3};

GLFWwindow *gear::Renderer::m_Window = nullptr;

void gear::Renderer::create(int width, int height)
{
  m_Window = glfwGetCurrentContext();
  
  int window_Width, window_Height;
  glfwGetWindowSize(m_Window, &window_Width, &window_Height);
  glViewport(0, 0, window_Width, window_Height);

  m_Framebuffer.create(width, height);

  create_Upscale_PL();

  create_Sprite_Nobatch_PL();
}

void gear::Renderer::destroy(void)
{
  m_Upscale_PL.destroy();
  m_Sprite_Nobatch_PL.destroy();

  m_Framebuffer.destroy();

  m_Window = nullptr;
}

/*
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
*/
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
  glBindTexture(GL_TEXTURE_2D, m_Framebuffer.m_Texture);
  m_Upscale_PL.bind();

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(m_Window);
}

void gear::Renderer::setup_Test_Frame(void)
{
  GEAR_DEBUG_LOG("setting up test texture");
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Framebuffer.m_Texture);

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

static void sprite_Render_Callback(gear::PositionComponent &position, gear::SpriteComponent &sprite)
{
  GEAR_DEBUG_LOG("rendering sprite entity");
}

void gear::Renderer::render_Scene(gear::Scene *scene)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer.m_Framebuffer);
  m_Sprite_Nobatch_PL.bind();

  gear::Entity::for_Each(scene->get_ID(), sprite_Render_Callback);
}

void gear::Renderer::create_Upscale_PL(void)
{
  unsigned int vertex_Shader = RenderPipeline::load_Shader("shaders/vs_upscale.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = RenderPipeline::load_Shader("shaders/fs_upscale.glsl", GL_FRAGMENT_SHADER);

  m_Upscale_PL.create(vertex_Shader, fragment_Shader);
  m_Upscale_PL.bind();
  glUniform1i(glGetUniformLocation(m_Upscale_PL.m_Shader, "u_Texture"), 0);
  GEAR_DEBUG_LOG("opengl program: %i", m_Upscale_PL.m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  glBindBuffer(GL_ARRAY_BUFFER, m_Upscale_PL.m_Vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), m_Default_Vertexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("vbo: %i", m_Upscale_PL.m_Vertexbuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Default_Indexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("ibo: %i", m_Upscale_PL.m_Indexbuffer);

  glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(0, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)));
  glVertexAttribBinding(1, 0);
  glEnableVertexAttribArray(1);
}

void gear::Renderer::create_Sprite_Nobatch_PL(void)
{
  unsigned int vertex_Shader = RenderPipeline::load_Shader("shaders/vs_sprite_nobatch.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = RenderPipeline::load_Shader("shaders/fs_sprite_nobatch.glsl", GL_FRAGMENT_SHADER);

  m_Sprite_Nobatch_PL.create(vertex_Shader, fragment_Shader);
  m_Sprite_Nobatch_PL.bind();
  glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Texture"), 0);
  GEAR_DEBUG_LOG("opengl program: %i", m_Sprite_Nobatch_PL.m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  glBindBuffer(GL_ARRAY_BUFFER, m_Sprite_Nobatch_PL.m_Vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), m_Default_Vertexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("vbo: %i", m_Sprite_Nobatch_PL.m_Vertexbuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Default_Indexbuffer, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("ibo: %i", m_Sprite_Nobatch_PL.m_Indexbuffer);

  glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
  glVertexAttribBinding(0, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, (2 * sizeof(float)));
  glVertexAttribBinding(1, 0);
  glEnableVertexAttribArray(1);
}