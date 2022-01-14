#include "SpriteBatchPipeline.h"

#include <glad/glad.h>

gear::SpriteBatchPipeline::~SpriteBatchPipeline() {
  delete[] m_Vertex_Data;
  delete[] m_Index_Data;
}

void gear::SpriteBatchPipeline::init(void)
{
  unsigned int vertex_Shader = RenderPipeline::load_Shader("shaders/vs_sprite_batch.glsl", GL_VERTEX_SHADER);
  unsigned int fragment_Shader = RenderPipeline::load_Shader("shaders/fs_sprite_batch.glsl", GL_FRAGMENT_SHADER);

  create(vertex_Shader, fragment_Shader);
  bind();
  GEAR_DEBUG_LOG("opengl program: %i", m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_Max_Textures);
  m_Max_Textures /= 2;

  m_Vertex_Data = new Vertex[4 * m_Max_Textures];
  m_Index_Data = new unsigned int[6 * m_Max_Textures];

  glBufferData(GL_ARRAY_BUFFER, m_Max_Textures * 4 * sizeof(Vertex), m_Default_Vertex_Data, GL_DYNAMIC_DRAW);
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

  glBindVertexBuffer(0, m_Vertexbuffer, 0, sizeof(Vertex));
}