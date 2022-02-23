#pragma once

#include <gear/core/core.h>
#include "Shape.h"

_GEAR_START

class Rect : public Shape
{
public:
  Vector<double, 2> top_Left, bottom_Right;

  Rect(const Vector<double, 2> &top_Left, const Vector<double, 2> &bottom_Right);

  void transform(TransformComponent *transform, Shape *transformed_Shape) const override;
};

_GEAR_END