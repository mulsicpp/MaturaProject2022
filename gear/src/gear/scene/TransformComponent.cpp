#include "TransformComponent.h"
#include "Entity.h"

static const gear::Matrix<double, 3, 3> flip_Axes = {
    0, 1, 0,
    1, 0, 0,
    0, 0, 1};

#define GEAR_TRANSLATE_MAT(x, y) \
  gear::Matrix<double, 3, 3> { 1, 0, x, 0, 1, y, 0, 0, 1 }
#define GEAR_SCALE_MAT(x, y) \
  gear::Matrix<double, 3, 3> { x, 0, 0, 0, y, 0, 0, 0, 1 }
#define GEAR_MIRROR_MAT(x, y) \
  gear::Matrix<double, 3, 3> { x, 0, 0, 0, y, 0, 0, 0, 1 }

gear::TransformComponent::TransformComponent(gear::Vector<double, 2> position, gear::Vector<double, 2> scale, uint8_t state)
    : position(position), scale(scale), state(state)
{
  update_Matrix();
}

void gear::TransformComponent::update_Matrix(void)
{
  m_Matrix = GEAR_TRANSLATE_MAT(position[0], position[1]);
  m_Matrix = m_Matrix * GEAR_SCALE_MAT(scale[0], scale[1]);
  m_Matrix = m_Matrix * GEAR_MIRROR_MAT(state & GEAR_MIRROR_X ? -1.0f : 1.0f, state & GEAR_MIRROR_Y ? -1.0f : 1.0f);
  if (state & GEAR_FLIP_AXES)
    m_Matrix = m_Matrix * flip_Axes;
}

gear::Matrix<double, 3, 3> gear::TransformComponent::get_Matrix(void) const { return m_Matrix; }