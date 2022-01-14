#include "Renderer.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>
#include <gear/data/FileStream.h>

#include <gear/scene/Entity.h>

gear::Framebuffer gear::Renderer::m_Framebuffer;

float gear::Renderer::m_Default_Vertexbuffer[16] = {
    -1.0, -1.0, 0.0, 0.0,
    1.0, -1.0, 1.0, 0.0,
    1.0, 1.0, 1.0, 1.0,
    -1.0, 1.0, 0.0, 1.0};

unsigned int gear::Renderer::m_Default_Indexbuffer[6] = {0, 1, 2, 0, 2, 3};

float gear::Renderer::m_Sprite_Nobatch_Vertexbuffer_Data[16]{0};
float *gear::Renderer::m_Sprite_Batch_Vertexbuffer_Data;

GLFWwindow *gear::Renderer::m_Window = nullptr;
int gear::Renderer::m_Window_Width;
int gear::Renderer::m_Window_Height;

int gear::Renderer::m_Max_Texture_Units;

void gear::Renderer::create(int width, int height)
{
  m_Window = glfwGetCurrentContext();
  glfwGetWindowSize(m_Window, &m_Window_Width, &m_Window_Height);
  glViewport(0, 0, m_Window_Width, m_Window_Height);

  m_Framebuffer.create(width, height);

  UpscalePipeline::get_Instance().init();
  SpriteNobatchPipeline::get_Instance().init();
}

void gear::Renderer::destroy(void)
{
  m_Framebuffer.destroy();

  m_Window = nullptr;
}

void gear::Renderer::set_V_Sync(bool v_sync)
{
  glfwSwapInterval(v_sync ? 1 : 0);
}

void gear::Renderer::start_New_Frame(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer.m_Framebuffer);
  glClear(GL_COLOR_BUFFER_BIT);
}

void gear::Renderer::show_Frame(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, m_Window_Width, m_Window_Height);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Framebuffer.m_Texture);
  UpscalePipeline::get_Instance().bind();

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(m_Window);
}

void gear::Renderer::render_Scene(gear::Scene *scene)
{
  m_Framebuffer.bind();
  SpriteNobatchPipeline::get_Instance().bind();
  glUniform1i(glGetUniformLocation(SpriteNobatchPipeline::get_Instance().m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
  glUniform1i(glGetUniformLocation(SpriteNobatchPipeline::get_Instance().m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);
  SpriteNobatchPipeline::get_Instance().render(scene);
}