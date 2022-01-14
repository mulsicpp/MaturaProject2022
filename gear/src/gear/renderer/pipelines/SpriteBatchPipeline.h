#pragma once

#include <gear/core/core.h>
#include "../RenderPipeline.h"
#include <gear/math/Vector.h>
#include <gear/scene/Scene.h>
#include <gear/scene/PositionComponent.h>
#include "../AnimationComponent.h"
#include "../SpriteComponent.h"

_GEAR_START

class SpriteBatchPipeline : public RenderPipeline {
private:
  struct Vertex {
    Vector<float, 3> pos;
    Vector<float, 2> tex;
    uint8_t tex_Index;
  };

  Vertex m_Temp_Vertex_Data[4];

  Vertex *m_Vertex_Data = nullptr;
  unsigned int *m_Index_Data = nullptr;

  int m_Max_Textures = 1;
  uint8_t m_Batch_Index = 0;

  static SpriteBatchPipeline instance;

  static void sprite_Render_Callback(PositionComponent &position, SpriteComponent &sprite);
  static void animation_Render_Callback(PositionComponent &position, AnimationComponent &animation);

public:
  static SpriteBatchPipeline &get_Instance(void);

  void init(void) override;
  void destroy(void) override;

  void render(Scene *scene);
  void draw_Batch(void);
};

_GEAR_END