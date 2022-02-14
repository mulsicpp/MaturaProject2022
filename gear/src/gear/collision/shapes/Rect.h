#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Rect : public Shape
{
public:
  Vector<float, 2> top_Left, bottom_Right;

  Rect(void);
  Rect(const Vector<float, 2> &top_Left, const Vector<float, 2> &bottom_Right);
};

_GEAR_END