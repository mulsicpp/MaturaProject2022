#include "Shape.h"
#include "collisions.h"

gear::Shape::Shape(ShapeType type) : m_Type(type) {}

gear::ShapeType gear::Shape::get_Type(void) const { return m_Type; }

template <class T>
static bool intersection_First_Type(const T *hitbox1, const gear::Shape *hitbox2)
{
  switch (hitbox2->get_Type())
  {
  case gear::ShapeType::RECT:
    return gear::shapes_Intersect(hitbox1, (gear::Rect *)hitbox2);
  case gear::ShapeType::CIRCLE:
    return gear::shapes_Intersect(hitbox1, (gear::Circle *)hitbox2);
  case gear::ShapeType::POINT:
    return gear::shapes_Intersect(hitbox1, (gear::Point *)hitbox2);
  default:
    return false;
  }
}

bool gear::Shape::intersects(const gear::Shape *shape) const
{
  const Shape *hitbox1, *hitbox2;
  if (this->m_Type > shape->m_Type)
  {
    hitbox1 = shape;
    hitbox2 = this;
  }
  else
  {
    hitbox1 = this;
    hitbox2 = shape;
  }
  switch (hitbox1->m_Type)
  {
  case ShapeType::RECT:
    return intersection_First_Type((Rect *)hitbox1, hitbox2);
  case ShapeType::CIRCLE:
    return intersection_First_Type((Circle *)hitbox1, hitbox2);
  case ShapeType::POINT:
    return intersection_First_Type((Point *)hitbox1, hitbox2);
  default:
    return false;
  }
}