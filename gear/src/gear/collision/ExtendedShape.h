#pragma once

#include <gear/core/core.h>
#include "shapes/Shape.h"

_GEAR_START

struct ExtendedShape
{
  Ref<Shape> shape;
  Ref<Shape> absolute_Shape;
  Ref<Shape> previous_Shape;
};

_GEAR_END