#pragma once

#include <gear/core/core.h>

_GEAR_START

class Renderer;

class RenderPipeline {
  friend class gear::Renderer;
private:
  unsigned int m_Shader = 0;
  unsigned int m_Vertexbuffer = 0;
  unsigned int m_Indexbuffer = 0;
  unsigned int m_Vertexarray = 0;

  void bind(void);
};

_GEAR_END