#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Point : public Shape {
public:
  Vector<double, 2> position;
  
  Point(void);
  Point(const Vector<double, 2> &pos, bool enabled = true);

  void transform(const TransformComponent *transform, Shape *transformed_Shape) const override;
};

_GEAR_END