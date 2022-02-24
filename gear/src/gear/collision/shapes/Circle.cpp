#include "Circle.h"

gear::Circle::Circle(const gear::Vector<double, 2> &position, double radius) : Shape(ShapeType::CIRCLE), position(position), radius(radius) {}

void gear::Circle::transform(const gear::TransformComponent *transform, gear::Shape *transformed_Shape) const 
{
  Circle *s = (Circle *)transformed_Shape;
  s->position = (transform->get_Matrix() * Vector<double, 3>{position[0], position[1], 1}).use_As<2>();
  s->radius = sqrt(transform->scale[0] * transform->scale[1]) * radius;
}