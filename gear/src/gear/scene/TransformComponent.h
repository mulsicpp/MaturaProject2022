#pragma once

#include <gear/core/core.h>
#include <gear/math/Matrix.h>

#define GEAR_MIRROR_X  0x01
#define GEAR_MIRROR_Y  0x02
#define GEAR_FLIP_AXES 0x04

_GEAR_START

struct TransformComponent {
  gear::Vector<float, 2> position = {0, 0};
  gear::Vector<float, 2> scale = {1, 1};
  uint8_t state = 0;

  gear::Matrix<float, 3, 3> get_Matrix(void);
};

_GEAR_END