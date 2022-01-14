#pragma once

#include <gear/core/core.h>
#include <gear/core/debug/log.h>

_GEAR_START

class Renderer;

class RenderVertex {
  
};

class RenderPipeline
{
  friend class gear::Renderer;

protected:
  unsigned int m_Shader = 0;
  unsigned int m_Vertexbuffer = 0;
  unsigned int m_Indexbuffer = 0;
  unsigned int m_Vertexarray = 0;
  
  void create(unsigned int vertex_Shader, unsigned int fragment_Shader);

public:
  static unsigned int load_Shader(const char *filename, unsigned int type);
  static unsigned int link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader);
  static bool validate_Program(unsigned int program);


  void bind(void);

  virtual void init(void) = 0;
  virtual void destroy(void);
};

_GEAR_END