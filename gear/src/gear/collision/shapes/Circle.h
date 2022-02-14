#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Circle : public Shape
{
public:
  Vector<float, 2> position;
  float radius;
  
  Circle(const Vector<float, 2> &position, float radius);
};

_GEAR_END