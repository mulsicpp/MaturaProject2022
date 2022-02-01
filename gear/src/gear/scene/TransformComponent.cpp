#include "TransformComponent.h"

static const gear::Matrix<float, 3, 3> flip_Axes = {
  0, 1, 0,
  1, 0, 0,
  0, 0, 1
};

#define GEAR_TRANSLATE_MAT(x, y) gear::Matrix<float, 3, 3>{1, 0, x, 0, 1, y, 0, 0, 1}
#define GEAR_SCALE_MAT(x, y)     gear::Matrix<float, 3, 3>{x, 0, 0, 0, y, 0, 0, 0, 1}
#define GEAR_MIRROR_MAT(x, y)    gear::Matrix<float, 3, 3>{x, 0, 0, 0, y, 0, 0, 0, 1}

gear::Matrix<float, 3, 3> gear::TransformComponent::get_Matrix(void)
{
  gear::Matrix<float, 3, 3> ret = GEAR_TRANSLATE_MAT(position[0], position[1]);
  ret = ret * GEAR_SCALE_MAT(scale[0], scale[1]);
  ret = ret * GEAR_MIRROR_MAT(state & GEAR_MIRROR_X ? -1.0f : 1.0f, state & GEAR_MIRROR_Y ? -1.0f : 1.0f);
  if(state & GEAR_FLIP_AXES)
    ret = ret * flip_Axes;

  return ret;
}