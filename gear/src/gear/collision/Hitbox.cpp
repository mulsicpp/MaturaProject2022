#include "Hitbox.h"

std::vector<gear::ExtendedShape> &gear::Hitbox::get_Shapes(void)
{
  return m_Shapes;
}

void gear::Hitbox::transform(gear::TransformComponent *transform) {
  for(auto &shape : m_Shapes)
    shape.shape->transform(transform, shape.absolute_Shape.get());
}