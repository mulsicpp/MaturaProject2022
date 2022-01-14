#pragma once

#include <gear/core/core.h>
#include "../RenderPipeline.h"
#include <gear/math/Vector.h>
#include <gear/scene/Scene.h>

#include <gear/scene/PositionComponent.h>
#include "../SpriteComponent.h"
#include "../AnimationComponent.h"

_GEAR_START

class SpriteNobatchPipeline : public RenderPipeline {
private:
  struct Vertex {
    Vector<float, 3> pos;
    Vector<float, 2> tex;
  };

  Vertex m_Vertex_Data[4];
  unsigned int m_Index_Data[6] = {0, 1, 2, 0, 2, 3};

  static SpriteNobatchPipeline instance;

  static void sprite_Render_Callback(PositionComponent &position, SpriteComponent &sprite);
  static void animation_Render_Callback(PositionComponent &position, AnimationComponent &animation);

public:
  static SpriteNobatchPipeline &get_Instance(void);

  void init(void) override;

  void render(Scene *scene);
};

_GEAR_END