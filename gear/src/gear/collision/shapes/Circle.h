#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Circle : public Shape
{
public:
  Vector<double, 2> position;
  double radius;
  
  Circle(void);
  Circle(const Circle &other) = default;
  Circle(const Vector<double, 2> &position, double radius, bool enabled = true);

  void transform(const TransformComponent *transform, Shape *transformed_Shape) const override;
  void assign(const Shape *other) override;
  Ref<Shape> copy(void) const override;
};

_GEAR_END