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
  Circle(const Vector<double, 2> &position, double radius, bool enabled = true);

  void transform(const TransformComponent *transform, Shape *transformed_Shape) const override;
  void assign(const Shape *other) override;
};

_GEAR_END