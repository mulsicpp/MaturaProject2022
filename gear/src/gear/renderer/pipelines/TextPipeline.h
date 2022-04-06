#pragma once

#include <gear/core/core.h>
#include "../RenderPipeline.h"
#include <gear/math/Vector.h>
#include <gear/scene/Scene.h>

#include <gear/scene/TransformComponent.h>
#include "../AnimationComponent.h"
#include "../SpriteComponent.h"

_GEAR_START

class TextPipeline : public RenderPipeline
{
private:
  struct Vertex
  {
    Vector<float, 3> pos;
    Vector<float, 2> tex;
    float parallax_Factor;
    Vector<uint8_t, 4> colors[8];
  };

  Vertex m_Temp_Vertex_Data[4];

  Vertex *m_Vertex_Data = nullptr;
  unsigned int *m_Index_Data = nullptr;

  int m_Max_Textures = 1;
  uint8_t m_Batch_Index = 0;

  static TextPipeline instance;

public:
  static TextPipeline &get_Instance(void);

  void init(void) override;
  void destroy(void) override;

  void render(Scene *scene);
};

_GEAR_END