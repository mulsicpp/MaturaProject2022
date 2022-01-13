#include "RenderPipeline.h"
#include <glad/glad.h>

#include <fstream>

void gear::RenderPipeline::create(unsigned int vertex_Shader, unsigned int fragment_Shader)
{
  m_Shader = link_Program(vertex_Shader, fragment_Shader);
  glCreateBuffers(1, &m_Vertexbuffer);
  glCreateBuffers(1, &m_Indexbuffer);
  glCreateVertexArrays(1, &m_Vertexarray);
}

gear::RenderPipeline::~RenderPipeline() {
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

unsigned int gear::RenderPipeline::load_Shader(const char *filename, unsigned int type)
{
  std::ifstream in = std::ifstream(filename);
  std::string shader_Source((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

  unsigned int id = glCreateShader(type);
  const char *src = shader_Source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int res = 0;

  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  if (res == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    gear::error("Shader compilation failed: %s", message);
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int gear::RenderPipeline::link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader)
{
  unsigned int program = glCreateProgram();

  glAttachShader(program, vertex_Shader);
  glAttachShader(program, fragment_Shader);
  glLinkProgram(program);
  glValidateProgram(program);

  int res;

  glGetProgramiv(program, GL_VALIDATE_STATUS, &res);
  if (res == GL_FALSE)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetProgramInfoLog(program, length, &length, message);
    gear::error("OpenGl program validation failed: %s", message);
    glDeleteShader(program);
    return 0;
  }

  return program;
}