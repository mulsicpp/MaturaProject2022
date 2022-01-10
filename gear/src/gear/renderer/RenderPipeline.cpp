#include "RenderPipeline.h"
#include <glad/glad.h>

void gear::RenderPipeline::bind(void)
{
  glUseProgram(m_Shader);
  glBindVertexArray(m_Vertexarray);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Indexbuffer);
}