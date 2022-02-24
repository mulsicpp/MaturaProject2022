#include "Point.h"

gear::Point::Point(const gear::Vector<double, 2> &position) : Shape(ShapeType::POINT), position(position) {}

void gear::Point::transform(const gear::TransformComponent *transform, gear::Shape *transformed_Shape) const 
{
  Point *s = (Point *)transformed_Shape;
  s->position = (transform->get_Matrix() * Vector<double, 3>{position[0], position[1], 1}).use_As<2>();
}