#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Circle : public Shape
{
public:
  Vector<double, 2> position;
  double radius;
  
  Circle(const Vector<double, 2> &position, double radius);

  void transform(TransformComponent *transform, Shape *transformed_Shape) const override;
};

_GEAR_END