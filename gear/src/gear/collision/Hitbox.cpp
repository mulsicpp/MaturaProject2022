#include "Hitbox.h"

std::vector<gear::ExtendedShape> &gear::Hitbox::get_Shapes(void)
{
  return m_Shapes;
}

void gear::Hitbox::transform(gear::TransformComponent *transform)
{
  for (auto &shape : m_Shapes)
    shape.shape->transform(transform, shape.absolute_Shape.get());
}

bool gear::Hitbox::intersects(const gear::Hitbox &other) const
{
  for (auto &shape1 : m_Shapes)
    for (auto &shape2 : other.m_Shapes)
      if (shape1.absolute_Shape->intersects(shape2.absolute_Shape.get()))
        return true;
  return false;
}

bool gear::Hitbox::intersected(const gear::Hitbox &other) const
{
  for (auto &shape1 : m_Shapes)
    for (auto &shape2 : other.m_Shapes)
      if (shape1.previous_Shape->intersects(shape2.previous_Shape.get()))
        return true;
  return false;
}

void gear::Hitbox::set_Previous(void)
{
  for (auto &shape : m_Shapes)
    shape.previous_Shape->assign(shape.absolute_Shape.get());
}