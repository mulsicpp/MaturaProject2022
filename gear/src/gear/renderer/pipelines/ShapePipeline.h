#pragma once

#include <gear/core/core.h>
#include "../RenderPipeline.h"
#include <gear/math/Vector.h>
#include <gear/scene/Scene.h>

#include <gear/scene/TransformComponent.h>
#include "../AnimationComponent.h"
#include "../SpriteComponent.h"
#include <gear/collision/shapes/Shape.h>
#include <gear/collision/shapes/Rect.h>
#include <gear/collision/shapes/Circle.h>
#include <gear/collision/shapes/Point.h>

_GEAR_START

class ShapePipeline : public RenderPipeline {
private:
  static ShapePipeline m_Instance;

  struct ShapeData {
    unsigned int vertex_Buffer;
    unsigned int index_Buffer;
  };

  unsigned int m_Rect_Data;
  unsigned int m_Circle_Data;
  unsigned int m_Point_Data;

public:
  static ShapePipeline &get_Instance(void);

  void init(void) override;
  void destroy(void) override;

  void bind(ShapeType type);

  void render_Shape(const Shape *shape, const Vector<float, 4> &color);

  void render_Rect(const Rect *circle);
  void render_Circle(const Circle *circle);
  void render_Point(const Point *circle);
};

_GEAR_END