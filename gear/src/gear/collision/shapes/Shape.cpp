#include "Shape.h"
#include "collisions.h"

gear::Shape::Shape(ShapeType type) : m_Type(type) {}

gear::ShapeType gear::Shape::get_Type(void) const { return m_Type; }

bool gear::Shape::is_Enabled(void) const
{
  return m_Enabled;
}

void gear::Shape::set_Enabled(bool enabled)
{
  m_Enabled = enabled;
}

template <class T>
static bool intersection_First_Type(const T *hitbox1, const gear::Shape *hitbox2, gear::Vector<double, 2> *separation_Vector)
{
  switch (hitbox2->get_Type())
  {
  case gear::ShapeType::RECT:
    return gear::shapes_Intersect(hitbox1, (gear::Rect *)hitbox2, separation_Vector);
  case gear::ShapeType::CIRCLE:
    return gear::shapes_Intersect(hitbox1, (gear::Circle *)hitbox2, separation_Vector);
  case gear::ShapeType::POINT:
    return gear::shapes_Intersect(hitbox1, (gear::Point *)hitbox2, separation_Vector);
  default:
    return false;
  }
}

bool gear::Shape::intersects(const gear::Shape *shape, gear::Vector<double, 2> *separation_Vector) const
{
  const Shape *hitbox1, *hitbox2;
  if(!m_Enabled || !shape->m_Enabled)
    return false;
  bool ret = false;
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
  case gear::ShapeType::RECT:
    ret =  intersection_First_Type((gear::Rect *)hitbox1, hitbox2, separation_Vector);
    break;
  case gear::ShapeType::CIRCLE:
    ret = intersection_First_Type((gear::Circle *)hitbox1, hitbox2, separation_Vector);
    break;
  case gear::ShapeType::POINT:
    ret = intersection_First_Type((gear::Point *)hitbox1, hitbox2, separation_Vector);
    break;
  }
  if(hitbox1 != this && separation_Vector)
    *separation_Vector = -*separation_Vector;
  return ret;
}