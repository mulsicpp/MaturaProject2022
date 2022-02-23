#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Point : public Shape {
public:
  Vector<double, 2> position;
  
  Point(const Vector<double, 2> &pos);

  void transform(TransformComponent *transform, Shape *transformed_Shape) const override;
};

_GEAR_END