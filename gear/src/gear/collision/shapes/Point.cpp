#include "Point.h"

#include <gear/core/debug/log.h>

gear::Point::Point(void) : Point({0, 0}) {}

gear::Point::Point(const gear::Vector<double, 2> &position, bool enabled) : Shape(ShapeType::POINT), position(position)
{
  set_Enabled(enabled);
}

void gear::Point::transform(const gear::TransformComponent *transform, gear::Shape *transformed_Shape) const 
{
  Point *s = (Point *)transformed_Shape;
  s->m_Enabled = m_Enabled;
  s->position = (transform->get_Matrix() * Vector<double, 3>{position[0], position[1], 1}).use_As<2>();
}

void gear::Point::assign(const gear::Shape *other)
{
  Shape::assign(other);
  if(get_Type() == other->get_Type()) {
    this->position = ((Point *)other)->position;
  }
}

gear::Ref<gear::Shape> gear::Point::copy(void) const {
  return Ref<Point>(new Point(*this));
}