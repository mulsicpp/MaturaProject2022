#pragma once

#include <gear/core/core.h>

_GEAR_START

class Renderer;

class RenderPipeline {
  friend class gear::Renderer;
private:
  static unsigned int load_Shader(const char *filename, unsigned int type);
  static unsigned int link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader);
  
private:
  unsigned int m_Shader = 0;
  unsigned int m_Vertexbuffer = 0;
  unsigned int m_Indexbuffer = 0;
  unsigned int m_Vertexarray = 0;

  void create(unsigned int vertex_Shader, unsigned int fragment_Shader);
  void destroy(void);

  void bind(void);

};

_GEAR_END