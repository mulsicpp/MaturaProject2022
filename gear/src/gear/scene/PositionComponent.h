#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>

_GEAR_START

struct PositionComponent {
  gear::Vector<float, 2> position;
};

_GEAR_END