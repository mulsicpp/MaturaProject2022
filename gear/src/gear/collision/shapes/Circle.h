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
};

_GEAR_END