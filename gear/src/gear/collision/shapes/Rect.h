#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Rect : public Shape
{
public:
  Vector<double, 2> top_Left, bottom_Right;

  Rect(void);
  Rect(const Rect &other) = default;
  Rect(const Vector<double, 2> &top_Left, const Vector<double, 2> &bottom_Right, bool enabled = true);

  void transform(const TransformComponent *transform, Shape *transformed_Shape) const override;
  void assign(const Shape *other) override;
  Ref<Shape> copy(void) const override;
};

_GEAR_END