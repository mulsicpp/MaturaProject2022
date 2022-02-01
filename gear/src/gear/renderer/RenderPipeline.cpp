#include "RenderPipeline.h"
#include "shaders.h"

#include <fstream>

void gear::RenderPipeline::create(unsigned int vertex_Shader, unsigned int fragment_Shader)
{
  m_Shader = link_Program(vertex_Shader, fragment_Shader);
  glCreateBuffers(1, &m_Vertexbuffer);
  glCreateBuffers(1, &m_Indexbuffer);
  glCreateVertexArrays(1, &m_Vertexarray);
}

void gear::RenderPipeline::destroy(void) {
  glDeleteProgram(m_Shader);
  glDeleteBuffers(1, &m_Vertexbuffer);
  glDeleteBuffers(1, &m_Indexbuffer);
  glDeleteVertexArrays(1, &m_Vertexarray);
}

void gear::RenderPipeline::bind(void)
{
  glUseProgram(m_Shader);
  glBindVertexArray(m_Vertexarray);
  glBindBuffer(GL_ARRAY_BUFFER, m_Vertexbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Indexbuffer);
}