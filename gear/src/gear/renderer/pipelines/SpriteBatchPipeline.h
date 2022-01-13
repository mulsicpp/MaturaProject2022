#pragma once

#include <gear/core/core.h>
#include "../RenderPipeline.h"
#include <gear/math/Vector.h>

_GEAR_START

class SpriteBatchPipeline : public RenderPipeline {
private:
  struct Vertex {
    Vector<float, 3> pos;
    Vector<float, 2> tex;
    uint8_t tex_Index;
  };

  Vertex m_Default_Vertex_Data[4] = {
    {{-1, -1, 0}, {0, 0}, 0},
    {{ 1, -1, 0}, {1, 0}, 0},
    {{ 1,  1, 0}, {1, 1}, 0},
    {{-1,  1, 0}, {0, 1}, 0}
  };

  Vertex *m_Vertex_Data = nullptr;
  unsigned int *m_Index_Data = nullptr;

public:
  void init(void) override;
};

_GEAR_END