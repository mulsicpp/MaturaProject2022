#pragma once

#include <gear/core/core.h>
#include <gear/math/Matrix.h>
#include "Scene.h"

#define GEAR_MIRROR_X 0x01
#define GEAR_MIRROR_Y 0x02
#define GEAR_FLIP_AXES 0x04

_GEAR_START

struct TransformComponent
{
  Vector<double, 2> position = {0, 0};
  Vector<double, 2> scale = {1, 1};
  uint8_t state = 0;

  TransformComponent(Vector<double, 2> position = Vector<double, 2>(0, 0),
                     Vector<double, 2> scale = Vector<double, 2>(1, 1),
                     uint8_t state = 0);

  Matrix<double, 3, 3> get_Matrix(void) const;
  void update_Matrix(void);

private:
  Matrix<double, 3, 3> m_Matrix;
};

void transform_Entities(Scene *scene);

_GEAR_END