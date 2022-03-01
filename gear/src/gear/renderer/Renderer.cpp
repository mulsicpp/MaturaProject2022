#include "Renderer.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>
#include <gear/data/FileStream.h>

#include <gear/scene/Entity.h>
#include "animation_player.h"

#include "pipelines/UpscalePipeline.h"
#include "pipelines/SpritePipeline.h"
#include "pipelines/ShapePipeline.h"

#include <gear/collision/PhysicsComponent.h>

using SpritePipeline = gear::SpritePipeline;

gear::Framebuffer gear::Renderer::m_Framebuffer;

gear::Camera *gear::Renderer::m_Camera = nullptr;

GLFWwindow *gear::Renderer::m_Window = nullptr;
int gear::Renderer::m_Window_Width;
int gear::Renderer::m_Window_Height;

void gear::Renderer::create(int width, int height)
{
  m_Window = glfwGetCurrentContext();
  glfwGetWindowSize(m_Window, &m_Window_Width, &m_Window_Height);
  glViewport(0, 0, m_Window_Width, m_Window_Height);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_GEQUAL);
  glClearDepth(-1.0);

  m_Framebuffer.create(width, height);

  UpscalePipeline::get_Instance().init();
  SpritePipeline::get_Instance().init();
  ShapePipeline::get_Instance().init();
}

void gear::Renderer::destroy(void)
{
  m_Framebuffer.destroy();

  UpscalePipeline::get_Instance().destroy();
  SpritePipeline::get_Instance().destroy();
  ShapePipeline::get_Instance().destroy();

  m_Window = nullptr;
}

void gear::Renderer::set_Camera(gear::Camera *camera)
{
  m_Camera = camera;
}

void gear::Renderer::set_V_Sync(bool v_sync)
{
  glfwSwapInterval(v_sync ? 1 : 0);
}

void gear::Renderer::start_New_Frame(void)
{
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer.m_Framebuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
  SpritePipeline::get_Instance().bind();
  glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
  glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);

  
  if(m_Camera != nullptr)
    glUniform2f(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Camera_Pos"), m_Camera->get_Position()[0], m_Camera->get_Position()[1]);
  else
    glUniform2f(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Camera_Pos"), 0, 0);


  SpritePipeline::get_Instance().render(scene);
  gear::Entity::for_Each(scene->get_ID(), animation_Player_Callback);
}

void gear::Renderer::render_Shape(const gear::Shape* shape, const Vector<float, 4> &color)
{
  m_Framebuffer.bind();
  ShapePipeline::get_Instance().bind(shape->get_Type());
  glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
  glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);

  
  if(m_Camera != nullptr)
    glUniform2f(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Camera_Pos"), m_Camera->get_Position()[0], m_Camera->get_Position()[1]);
  else
    glUniform2f(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Camera_Pos"), 0, 0);
  ShapePipeline::get_Instance().render_Shape(shape, color);
}

static void physics_Hitbox_Render_Callback(gear::PhysicsComponent &collider) {
  for(auto &hitbox : collider.collider.get_Shapes())
    gear::Renderer::render_Shape(hitbox.absolute_Shape.get(), {0, 1, 1, 1});
}

void gear::Renderer::render_All_Hitboxes(gear::Scene *scene)
{
  gear::Entity::for_Each(scene->get_ID(), physics_Hitbox_Render_Callback);
}