#include "SpriteNobatchPipeline.h"
#include <gear/scene/Entity.h>

#include <glad/glad.h>

gear::SpriteNobatchPipeline gear::SpriteNobatchPipeline::instance;

gear::SpriteNobatchPipeline &gear::SpriteNobatchPipeline::get_Instance(void)
{
  return instance;
}

void gear::SpriteNobatchPipeline::init(void)
{
  unsigned int vertex_Shader = RenderPipeline::load_Shader("shaders/vs_sprite_nobatch.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = RenderPipeline::load_Shader("shaders/fs_sprite_nobatch.glsl", GL_FRAGMENT_SHADER);

  create(vertex_Shader, fragment_Shader);
  RenderPipeline::validate_Program(m_Shader);
  bind();
  GEAR_DEBUG_LOG("opengl program: %i", m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
  GEAR_DEBUG_LOG("vbo: %i", m_Vertexbuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Index_Data, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("ibo: %i", m_Indexbuffer);

  glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
  glVertexAttribBinding(0, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));
  glVertexAttribBinding(1, 0);
  glEnableVertexAttribArray(1);

  glBindVertexBuffer(0, m_Vertexbuffer, 0, sizeof(Vertex));
}

void gear::SpriteNobatchPipeline::render(gear::Scene *scene) {
  bind();
  gear::Entity::for_Each(scene->get_ID(), sprite_Render_Callback);
  gear::Entity::for_Each(scene->get_ID(), animation_Render_Callback);
}

void gear::SpriteNobatchPipeline::sprite_Render_Callback(gear::PositionComponent &position, gear::SpriteComponent &sprite)
{
  float x_Left = position.position[0] + sprite.offset[0];
  float x_Right = position.position[0] + sprite.offset[0] + sprite.sprite->get_Width();
  float y_Top = position.position[1] + sprite.offset[1];
  float y_Bottom = position.position[1] + sprite.offset[1] + sprite.sprite->get_Height();

  instance.m_Vertex_Data[0] = {{x_Left, y_Top, 0}, {0, 0}};
  instance.m_Vertex_Data[1] = {{x_Right, y_Top, 0}, {1, 0}};
  instance.m_Vertex_Data[2] = {{x_Right, y_Bottom, 0}, {1, 1}};
  instance.m_Vertex_Data[3] = {{x_Left, y_Bottom, 0}, {0, 1}};

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sprite.sprite->get_TextureID());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, sprite.palette->get_TextureID());

  glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex), instance.m_Vertex_Data);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void gear::SpriteNobatchPipeline::animation_Render_Callback(gear::PositionComponent &position, gear::AnimationComponent &animation)
{
  float x_Left = position.position[0] + animation.offset[0];
  float x_Right = position.position[0] + animation.offset[0] + animation.animation->get_Width();
  float y_Top = position.position[1] + animation.offset[1];
  float y_Bottom = position.position[1] + animation.offset[1] + animation.animation->get_Height();

  int frame_Index = animation.animation_Offset;

  float tex_Top = float(frame_Index) / animation.animation->get_Frame_Count();
  float tex_Bottom = float(frame_Index + 1) / animation.animation->get_Frame_Count();

  instance.m_Vertex_Data[0] = {{x_Left, y_Top, 0}, {0, tex_Top}};
  instance.m_Vertex_Data[1] = {{x_Right, y_Top, 0}, {1, tex_Top}};
  instance.m_Vertex_Data[2] = {{x_Right, y_Bottom, 0}, {1, tex_Bottom}};
  instance.m_Vertex_Data[3] = {{x_Left, y_Bottom, 0}, {0, tex_Bottom}};

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, animation.animation->get_TextureID());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, animation.palette->get_TextureID());

  glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(Vertex), instance.m_Vertex_Data);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}