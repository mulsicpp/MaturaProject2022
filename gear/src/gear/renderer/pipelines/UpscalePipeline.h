#pragma once

#include <gear/core/core.h>
#include "../RenderPipeline.h"
#include <gear/math/Vector.h>

_GEAR_START

class UpscalePipeline : public RenderPipeline {
private:
  struct Vertex{
    Vector<float, 2> pos;
    Vector<float, 2> tex;
  };

  Vertex m_Default_Vertex_Data[4] = {
    {{-1, -1}, {0, 0}},
    {{ 1, -1}, {1, 0}},
    {{ 1,  1}, {1, 1}},
    {{-1,  1}, {0, 1}}
  };

  unsigned int m_Index_Data[6] = {0, 1, 2, 0, 2, 3};

  static UpscalePipeline instance;

public:
  static UpscalePipeline &get_Instance(void);

  void init(void) override;
};

_GEAR_END