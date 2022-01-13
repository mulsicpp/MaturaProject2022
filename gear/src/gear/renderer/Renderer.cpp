#include "Renderer.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>
#include <gear/data/FileStream.h>
#include <gear/scene/PositionComponent.h>

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

float gear::Renderer::m_Sprite_Nobatch_Vertexbuffer_Data[16]{0};

GLFWwindow *gear::Renderer::m_Window = nullptr;
int gear::Renderer::m_Window_Width;
int gear::Renderer::m_Window_Height;

void gear::Renderer::create(int width, int height)
{
  m_Window = glfwGetCurrentContext();
  glfwGetWindowSize(m_Window, &m_Window_Width, &m_Window_Height);
  glViewport(0, 0, m_Window_Width, m_Window_Height);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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
  m_Upscale_PL.bind();

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(m_Window);
}

void gear::Renderer::sprite_Render_Callback(gear::PositionComponent &position, gear::SpriteComponent &sprite)
{
  memcpy(m_Sprite_Nobatch_Vertexbuffer_Data, m_Default_Vertexbuffer, 16);

  m_Sprite_Nobatch_Vertexbuffer_Data[0] = position.position[0] + sprite.offset[0];
  m_Sprite_Nobatch_Vertexbuffer_Data[12] = m_Sprite_Nobatch_Vertexbuffer_Data[0];

  m_Sprite_Nobatch_Vertexbuffer_Data[4] = position.position[0] + sprite.offset[0] + sprite.sprite->get_Width();
  m_Sprite_Nobatch_Vertexbuffer_Data[8] = m_Sprite_Nobatch_Vertexbuffer_Data[4];

  m_Sprite_Nobatch_Vertexbuffer_Data[1] = position.position[1] + sprite.offset[1];
  m_Sprite_Nobatch_Vertexbuffer_Data[5] = m_Sprite_Nobatch_Vertexbuffer_Data[1];

  m_Sprite_Nobatch_Vertexbuffer_Data[9] = position.position[1] + sprite.offset[1] + sprite.sprite->get_Height();
  m_Sprite_Nobatch_Vertexbuffer_Data[13] = m_Sprite_Nobatch_Vertexbuffer_Data[9];

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sprite.sprite->get_TextureID());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, sprite.palette->get_TextureID());

  glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), m_Sprite_Nobatch_Vertexbuffer_Data);
  //glBindVertexBuffer(0, m_Sprite_Nobatch_PL.m_Vertexbuffer, 0, 4 * sizeof(float));

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  //GEAR_DEBUG_LOG("rendering sprite entity");
}

void gear::Renderer::animation_Render_Callback(gear::PositionComponent &position, gear::AnimationComponent &animation)
{
  memcpy(m_Sprite_Nobatch_Vertexbuffer_Data, m_Default_Vertexbuffer, 16);

  m_Sprite_Nobatch_Vertexbuffer_Data[0] = position.position[0] + animation.offset[0];
  m_Sprite_Nobatch_Vertexbuffer_Data[12] = m_Sprite_Nobatch_Vertexbuffer_Data[0];

  m_Sprite_Nobatch_Vertexbuffer_Data[4] = position.position[0] + animation.offset[0] + animation.animation->get_Width();
  m_Sprite_Nobatch_Vertexbuffer_Data[8] = m_Sprite_Nobatch_Vertexbuffer_Data[4];

  m_Sprite_Nobatch_Vertexbuffer_Data[1] = position.position[1] + animation.offset[1];
  m_Sprite_Nobatch_Vertexbuffer_Data[5] = m_Sprite_Nobatch_Vertexbuffer_Data[1];

  m_Sprite_Nobatch_Vertexbuffer_Data[9] = position.position[1] + animation.offset[1] + animation.animation->get_Height();
  m_Sprite_Nobatch_Vertexbuffer_Data[13] = m_Sprite_Nobatch_Vertexbuffer_Data[9];

  int frame_Index = animation.animation_Offset;

  m_Sprite_Nobatch_Vertexbuffer_Data[3] = float(frame_Index) / animation.animation->get_Frame_Count();
  m_Sprite_Nobatch_Vertexbuffer_Data[7] = m_Sprite_Nobatch_Vertexbuffer_Data[3];

  m_Sprite_Nobatch_Vertexbuffer_Data[11] = float(frame_Index + 1) / animation.animation->get_Frame_Count();
  m_Sprite_Nobatch_Vertexbuffer_Data[15] = m_Sprite_Nobatch_Vertexbuffer_Data[11];

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, animation.animation->get_TextureID());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, animation.palette->get_TextureID());

  glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), m_Sprite_Nobatch_Vertexbuffer_Data);
  //glBindVertexBuffer(0, m_Sprite_Nobatch_PL.m_Vertexbuffer, 0, 4 * sizeof(float));

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  animation.animation_Offset += animation.frame_Rate / 60.0f;
  animation.animation_Offset = int(animation.animation_Offset) % animation.animation->get_Frame_Count() + animation.animation_Offset - int(animation.animation_Offset);

  GEAR_DEBUG_LOG("rendering animation entity");
}

void gear::Renderer::render_Scene(gear::Scene *scene)
{
  m_Framebuffer.bind();
  m_Sprite_Nobatch_PL.bind();
  glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
  glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);
  glActiveTexture(GL_TEXTURE0);

  gear::Entity::for_Each(scene->get_ID(), sprite_Render_Callback);
  gear::Entity::for_Each(scene->get_ID(), animation_Render_Callback);
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

  glBindVertexBuffer(0, m_Upscale_PL.m_Vertexbuffer, 0, 4 * sizeof(float));
}

void gear::Renderer::create_Sprite_Nobatch_PL(void)
{
  unsigned int vertex_Shader = RenderPipeline::load_Shader("shaders/vs_sprite_nobatch.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = RenderPipeline::load_Shader("shaders/fs_sprite_nobatch.glsl", GL_FRAGMENT_SHADER);

  m_Sprite_Nobatch_PL.create(vertex_Shader, fragment_Shader);
  m_Sprite_Nobatch_PL.bind();
  glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Texture"), 0);
  glUniform1i(glGetUniformLocation(m_Sprite_Nobatch_PL.m_Shader, "u_Palette"), 1);
  GEAR_DEBUG_LOG("opengl program: %i", m_Sprite_Nobatch_PL.m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

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

  glBindVertexBuffer(0, m_Sprite_Nobatch_PL.m_Vertexbuffer, 0, 4 * sizeof(float));
}