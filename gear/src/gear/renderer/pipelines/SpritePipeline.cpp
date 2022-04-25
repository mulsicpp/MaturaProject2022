#include "SpritePipeline.h"

#include <gear/scene/Entity.h>

#include "../shaders.h"

gear::SpritePipeline gear::SpritePipeline::instance;

gear::SpritePipeline &gear::SpritePipeline::get_Instance(void)
{
  return instance;
}

void gear::SpritePipeline::destroy(void)
{
  delete[] m_Vertex_Data;
  delete[] m_Index_Data;

  RenderPipeline::destroy();
}

void gear::SpritePipeline::init(void)
{
  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_Max_Textures);
  m_Max_Textures /= 2;
  if (m_Max_Textures > 64)
    m_Max_Textures = 64;
  
  GEAR_DEBUG_LOG("max textures: %i", m_Max_Textures);

  unsigned int vertex_Shader = get_Shader(SHADER_SPRITE_VS, GL_VERTEX_SHADER);
  unsigned int fragment_Shader = get_Shader(SHADER_SPRITE_FS, GL_FRAGMENT_SHADER, m_Max_Textures, m_Max_Textures);

  create(vertex_Shader, fragment_Shader);
  bind();

  int texture_Bindings[64];
  int palette_Bindings[64];

  for (int i = 0; i < m_Max_Textures; i++)
  {
    texture_Bindings[i] = 2 * i;
    palette_Bindings[i] = 2 * i + 1;
  }

  glUniform1iv(glGetUniformLocation(m_Shader, "u_Texture"), m_Max_Textures, texture_Bindings);
  glUniform1iv(glGetUniformLocation(m_Shader, "u_Palette"), m_Max_Textures, palette_Bindings);

  validate_Program(m_Shader);

  GEAR_DEBUG_LOG("opengl program: %i", m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  m_Vertex_Data = new Vertex[4 * m_Max_Textures];
  m_Index_Data = new unsigned int[6 * m_Max_Textures];
  for (int i = 0; i < m_Max_Textures; i++)
  {
    m_Index_Data[i * 6 + 0] = 0 + i * 4;
    m_Index_Data[i * 6 + 1] = 1 + i * 4;
    m_Index_Data[i * 6 + 2] = 2 + i * 4;
    m_Index_Data[i * 6 + 3] = 0 + i * 4;
    m_Index_Data[i * 6 + 4] = 2 + i * 4;
    m_Index_Data[i * 6 + 5] = 3 + i * 4;
  }

  glBufferData(GL_ARRAY_BUFFER, m_Max_Textures * 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
  GEAR_DEBUG_LOG("vbo: %i", m_Vertexbuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Max_Textures * 6 * sizeof(unsigned int), m_Index_Data, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("ibo: %i", m_Indexbuffer);


  glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
  glVertexAttribBinding(0, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));
  glVertexAttribBinding(1, 0);
  glEnableVertexAttribArray(1);
  glVertexAttribFormat(2, 1, GL_UNSIGNED_BYTE, GL_FALSE, offsetof(Vertex, tex_Index));
  glVertexAttribBinding(2, 0);
  glEnableVertexAttribArray(2);
  glVertexAttribFormat(3, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, parallax_Factor));
  glVertexAttribBinding(3, 0);
  glEnableVertexAttribArray(3);

  glBindVertexBuffer(0, m_Vertexbuffer, 0, sizeof(Vertex));
}

void gear::SpritePipeline::render(gear::Scene *scene)
{
  bind();
  gear::Entity::for_Each(scene->get_ID(), push_Sprite_Quad);
  gear::Entity::for_Each(scene->get_ID(), push_Animation_Quad);
  draw_Batch();
}

void gear::SpritePipeline::render_UI(gear::Ref<gear::UIContainer> container)
{
  bind();
  for(auto &comp : container->m_Components)
    push_UI_Quad(comp);
  draw_Batch();
}

void gear::SpritePipeline::draw_Batch(void)
{
  if (m_Batch_Index > 0)
  {
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_Batch_Index * 4 * sizeof(Vertex), m_Vertex_Data);
    glDrawElements(GL_TRIANGLES, 6 * m_Batch_Index, GL_UNSIGNED_INT, nullptr);
    m_Batch_Index = 0;
  }
}

void gear::SpritePipeline::push_Sprite_Quad(gear::Entity parent, gear::TransformComponent &position, gear::SpriteComponent &sprite)
{
  double x_Left = sprite.offset[0];
  double x_Right = sprite.offset[0] + sprite.sprite->get_Width();
  double y_Top = sprite.offset[1];
  double y_Bottom = sprite.offset[1] + sprite.sprite->get_Height();

  double depth = sprite.offset[2];

  auto mat = position.get_Matrix();

  Vector<float, 3> pos = (mat * Vector<double, 3>{x_Left, y_Top, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[0] = {{pos[0], pos[1], depth}, {0, 0}, instance.m_Batch_Index, sprite.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Right, y_Top, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[1] = {{pos[0], pos[1], depth}, {1, 0}, instance.m_Batch_Index, sprite.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Right, y_Bottom, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[2] = {{pos[0], pos[1], depth}, {1, 1}, instance.m_Batch_Index, sprite.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Left, y_Bottom, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[3] = {{pos[0], pos[1], depth}, {0, 1}, instance.m_Batch_Index, sprite.parallax_Factor};

  glActiveTexture(GL_TEXTURE0 + 2 * instance.m_Batch_Index);
  glBindTexture(GL_TEXTURE_2D, sprite.sprite->get_TextureID());
  glActiveTexture(GL_TEXTURE1 + 2 * instance.m_Batch_Index);
  glBindTexture(GL_TEXTURE_1D, sprite.palette->get_TextureID());

  memcpy(instance.m_Vertex_Data + (4 * instance.m_Batch_Index), instance.m_Temp_Vertex_Data, 4);

  instance.m_Batch_Index++;
  if (instance.m_Batch_Index == instance.m_Max_Textures)
  {
    instance.draw_Batch();
  }
}

void gear::SpritePipeline::push_Animation_Quad(gear::Entity parent, gear::TransformComponent &position, gear::AnimationComponent &animation)
{
  float x_Left = animation.offset[0];
  float x_Right = animation.offset[0] + animation.animation->get_Width();
  float y_Top = animation.offset[1];
  float y_Bottom = animation.offset[1] + animation.animation->get_Height();

  float depth = animation.offset[2];

  int frame_Index = animation.frame_Offset;

  float tex_Top = float(frame_Index) / animation.animation->get_Frame_Count();
  float tex_Bottom = float(frame_Index + 1) / animation.animation->get_Frame_Count();

  auto mat = position.get_Matrix();

  Vector<float, 3> pos = (mat * Vector<double, 3>{x_Left, y_Top, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[0] = {{pos[0], pos[1], depth}, {0, tex_Top}, instance.m_Batch_Index, animation.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Right, y_Top, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[1] = {{pos[0], pos[1], depth}, {1, tex_Top}, instance.m_Batch_Index, animation.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Right, y_Bottom, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[2] = {{pos[0], pos[1], depth}, {1, tex_Bottom}, instance.m_Batch_Index, animation.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Left, y_Bottom, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[3] = {{pos[0], pos[1], depth}, {0, tex_Bottom}, instance.m_Batch_Index, animation.parallax_Factor};

  glActiveTexture(GL_TEXTURE0 + 2 * instance.m_Batch_Index);
  glBindTexture(GL_TEXTURE_2D, animation.animation->get_TextureID());
  glActiveTexture(GL_TEXTURE1 + 2 * instance.m_Batch_Index);
  glBindTexture(GL_TEXTURE_1D, animation.palette->get_TextureID());

  memcpy(instance.m_Vertex_Data + (4 * instance.m_Batch_Index), instance.m_Temp_Vertex_Data, 4);

  instance.m_Batch_Index++;
  if (instance.m_Batch_Index == instance.m_Max_Textures)
  {
    instance.draw_Batch();
  }
}

void gear::SpritePipeline::push_UI_Quad(gear::Ref<gear::UIComponent> component)
{
  float x_Left = component->m_Displayed_Animation.offset[0];
  float x_Right = component->m_Displayed_Animation.offset[0] + component->m_Displayed_Animation.animation->get_Width();
  float y_Top = component->m_Displayed_Animation.offset[1];
  float y_Bottom = component->m_Displayed_Animation.offset[1] + component->m_Displayed_Animation.animation->get_Height();

  float depth = component->m_Displayed_Animation.offset[2];

  int frame_Index = component->m_Displayed_Animation.frame_Offset;

  float tex_Top = float(frame_Index) / component->m_Displayed_Animation.animation->get_Frame_Count();
  float tex_Bottom = float(frame_Index + 1) / component->m_Displayed_Animation.animation->get_Frame_Count();

  auto mat = component->m_Transform.get_Matrix();

  Vector<float, 3> pos = (mat * Vector<double, 3>{x_Left, y_Top, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[0] = {{pos[0], pos[1], depth}, {0, tex_Top}, instance.m_Batch_Index, component->m_Displayed_Animation.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Right, y_Top, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[1] = {{pos[0], pos[1], depth}, {1, tex_Top}, instance.m_Batch_Index, component->m_Displayed_Animation.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Right, y_Bottom, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[2] = {{pos[0], pos[1], depth}, {1, tex_Bottom}, instance.m_Batch_Index, component->m_Displayed_Animation.parallax_Factor};
  pos = (mat * Vector<double, 3>{x_Left, y_Bottom, 1}).cast_To<float, 3>();
  instance.m_Temp_Vertex_Data[3] = {{pos[0], pos[1], depth}, {0, tex_Bottom}, instance.m_Batch_Index, component->m_Displayed_Animation.parallax_Factor};

  glActiveTexture(GL_TEXTURE0 + 2 * instance.m_Batch_Index);
  glBindTexture(GL_TEXTURE_2D, component->m_Displayed_Animation.animation->get_TextureID());
  glActiveTexture(GL_TEXTURE1 + 2 * instance.m_Batch_Index);
  glBindTexture(GL_TEXTURE_1D, component->m_Displayed_Animation.palette->get_TextureID());

  memcpy(instance.m_Vertex_Data + (4 * instance.m_Batch_Index), instance.m_Temp_Vertex_Data, 4);

  instance.m_Batch_Index++;
  if (instance.m_Batch_Index == instance.m_Max_Textures)
  {
    instance.draw_Batch();
  }
}