#include "UpscalePipeline.h"

#include "../shaders.h"

gear::UpscalePipeline gear::UpscalePipeline::instance;

gear::UpscalePipeline &gear::UpscalePipeline::get_Instance(void)
{
  return instance;
}

void gear::UpscalePipeline::init(void)
{
  unsigned int vertex_Shader = get_Shader(SHADER_UPSCALE_VS, GL_VERTEX_SHADER);
  unsigned int fragment_Shader = get_Shader(SHADER_UPSCALE_FS, GL_FRAGMENT_SHADER);

  create(vertex_Shader, fragment_Shader);
  validate_Program(m_Shader);
  bind();
  glUniform1i(glGetUniformLocation(m_Shader, "u_Texture"), 0);
  GEAR_DEBUG_LOG("opengl program: %i", m_Shader);

  glDeleteShader(vertex_Shader);
  glDeleteShader(fragment_Shader);

  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), m_Default_Vertex_Data, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("vbo: %i", m_Vertexbuffer);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Index_Data, GL_STATIC_DRAW);
  GEAR_DEBUG_LOG("ibo: %i", m_Indexbuffer);

  glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
  glVertexAttribBinding(0, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));
  glVertexAttribBinding(1, 0);
  glEnableVertexAttribArray(1);

  glBindVertexBuffer(0, m_Vertexbuffer, 0, sizeof(Vertex));
}