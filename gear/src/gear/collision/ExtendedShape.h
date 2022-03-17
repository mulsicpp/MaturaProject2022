#pragma once

#include <gear/core/core.h>
#include "shapes/Shape.h"

_GEAR_START

struct ExtendedShape
{
  Ref<Shape> shape;
  Ref<Shape> absolute_Shape;
  Ref<Shape> previous_Shape;

  ExtendedShape(void);
  ExtendedShape(Ref<Shape> shape, Ref<Shape> absolute_Shape, Ref<Shape> previous_Shape);
  ExtendedShape(const ExtendedShape &shape);
  ExtendedShape &operator=(const ExtendedShape &shape);
};

_GEAR_END