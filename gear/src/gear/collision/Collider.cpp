#include "Collider.h"

std::vector<gear::ExtendedShape> &gear::Collider::get_Shapes(void)
{
  return m_Shapes;
}

void gear::Collider::transform(gear::TransformComponent *transform)
{
  for (auto &shape : m_Shapes)
    shape.shape->transform(transform, shape.absolute_Shape.get());
}

bool gear::Collider::intersects(const gear::Collider &other) const
{
  for (auto &shape1 : m_Shapes)
    for (auto &shape2 : other.m_Shapes)
      if (shape1.absolute_Shape->intersects(shape2.absolute_Shape.get()))
        return true;
  return false;
}

bool gear::Collider::intersected(const gear::Collider &other) const
{
  for (auto &shape1 : m_Shapes)
    for (auto &shape2 : other.m_Shapes)
      if (shape1.previous_Shape->intersects(shape2.previous_Shape.get()))
        return true;
  return false;
}

void gear::Collider::set_Previous(void)
{
  for (auto &shape : m_Shapes)
    shape.previous_Shape->assign(shape.absolute_Shape.get());
}

void gear::Collider::set_Enabled(bool enabled)
{
  for (auto &shape : m_Shapes)
    shape.shape->set_Enabled(enabled);
}