#pragma once

#include <gear/core/core.h>
#include <gear/scene/TransformComponent.h>

_GEAR_START

enum class ShapeType
{
  INVALID,
  RECT,
  CIRCLE,
  POINT,
  LINE,
  CAPSULE
};

class Shape
{
private:
  const ShapeType m_Type;
  bool m_Enabled = true;

public:
  Shape(ShapeType type);
  virtual ~Shape() = default;

  ShapeType get_Type(void) const;

  bool is_Enabled(void) const;
  void set_Enabled(bool enabled);

  bool intersects(const Shape *other, Vector<double, 2> *separation_Vector = nullptr) const;

  virtual void transform(const TransformComponent *transform, Shape *transformed_Shape) const = 0;
};

_GEAR_END