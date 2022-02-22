#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Point : public Shape {
public:
  Vector<double, 2> position;
  
  Point(const Vector<double, 2> &pos);

  //bool intersects(Shape *other) override;
  //void transform(TransformComponent *transform, Shape *transformed_Shape) override;
};

_GEAR_END