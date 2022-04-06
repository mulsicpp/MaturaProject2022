#include "TextPipeline.h"

#include <gear/scene/Entity.h>

#include "../shaders.h"

gear::TextPipeline gear::TextPipeline::instance;

gear::TextPipeline &gear::TextPipeline::get_Instance(void)
{
  return instance;
}

void gear::TextPipeline::destroy(void)
{
  delete[] m_Vertex_Data;
  delete[] m_Index_Data;
}

void gear::TextPipeline::init(void)
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
  glVertexAttribFormat(2, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, parallax_Factor));
  glVertexAttribBinding(2, 0);
  glEnableVertexAttribArray(2);

  glBindVertexBuffer(0, m_Vertexbuffer, 0, sizeof(Vertex));
}

void gear::TextPipeline::render(gear::Scene *scene) {

}