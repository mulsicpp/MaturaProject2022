#pragma once

#include <gear/core/core.h>

_GEAR_START

class Renderer;

class Framebuffer {
  friend class gear::Renderer;
private:
  unsigned int m_Framebuffer;
  unsigned int m_Texture;
  unsigned int m_Depthbuffer;
  unsigned int m_Width, m_Height;

public:
  void create(int width, int height);
  void destroy(void);

  void set_Size(int width, int height);
  
  void bind(void);
};

_GEAR_END