#include "Circle.h"

#include <gear/core/debug/log.h>

gear::Circle::Circle(void) : Circle({0, 0}, 1) {}

gear::Circle::Circle(const gear::Vector<double, 2> &position, double radius, bool enabled)
    : Shape(ShapeType::CIRCLE), position(position), radius(radius)
{
  set_Enabled(enabled);
}

void gear::Circle::transform(const gear::TransformComponent *transform, gear::Shape *transformed_Shape) const
{
  Circle *s = (Circle *)transformed_Shape;
  s->m_Enabled = m_Enabled;
  s->position = (transform->get_Matrix() * Vector<double, 3>{position[0], position[1], 1}).use_As<2>();
  s->radius = sqrt(abs(transform->scale[0] * transform->scale[1])) * radius;
}

void gear::Circle::assign(const gear::Shape *other)
{
  Shape::assign(other);
  if(get_Type() == other->get_Type()) {
    this->position = ((Circle *)other)->position;
    this->radius = ((Circle *)other)->radius;
  }
}

gear::Ref<gear::Shape> gear::Circle::copy(void) const {
  return Ref<Circle>(new Circle(*this));
}