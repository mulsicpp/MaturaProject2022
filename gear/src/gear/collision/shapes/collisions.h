#include "Shape.h"
#include "Rect.h"
#include "Circle.h"
#include "Point.h"

#include <gear/core/debug/log.h>

_GEAR_START

template <class T1, class T2>
bool shapes_Intersect(const T1 *hitbox1, const T2 *hitbox2)
{
  gear::error("A parameter is not a shape");
  return false;
}

template <>
bool shapes_Intersect<Rect, Rect>(const Rect *hitbox1, const Rect *hitbox2)
{
  return !(hitbox2->top_Left[0] > hitbox1->bottom_Right[0] ||
           hitbox2->bottom_Right[0] < hitbox1->top_Left[0] ||
           hitbox2->top_Left[1] > hitbox1->bottom_Right[1] ||
           hitbox2->bottom_Right[1] < hitbox1->top_Left[1]);
}

template <>
bool shapes_Intersect<Rect, Circle>(const Rect *hitbox1, const Circle *hitbox2)
{
  float r_Left = hitbox1->top_Left[0],
        r_Right = hitbox1->bottom_Right[0],
        r_Top = hitbox1->top_Left[1],
        r_Bottom = hitbox1->bottom_Right[1],
        c_X = hitbox2->position[0],
        c_Y = hitbox2->position[1],
        c_Rad = hitbox2->radius;

  if (c_X >= r_Left - c_Rad && c_X <= r_Right + c_Rad && c_Y >= r_Top && c_Y <= r_Bottom)
    return true;
  if (c_X >= r_Left && c_X <= r_Right && c_Y >= r_Top - c_Rad && c_Y <= r_Bottom + c_Rad)
    return true;

  if ((hitbox2->position - hitbox1->top_Left).mag() <= c_Rad)
    return true;
  if ((hitbox2->position - hitbox1->bottom_Right).mag() <= c_Rad)
    return true;
  if ((hitbox2->position - Vector<float, 2>{r_Right, r_Top}).mag() <= c_Rad)
    return true;
  if ((hitbox2->position - Vector<float, 2>{r_Left, r_Bottom}).mag() <= c_Rad)
    return true;

  return false;
}

template <>
bool shapes_Intersect<Rect, Point>(const Rect *hitbox1, const Point *hitbox2)
{
  return (hitbox1->top_Left[0] <= hitbox2->position[0] &&
          hitbox1->bottom_Right[0] >= hitbox2->position[0] &&
          hitbox1->top_Left[1] <= hitbox2->position[1] &&
          hitbox1->bottom_Right[1] >= hitbox2->position[1]);
}

template <>
bool shapes_Intersect<Circle, Circle>(const Circle *hitbox1, const Circle *hitbox2)
{
  return (hitbox1->position - hitbox2->position).mag() <= (hitbox1->radius + hitbox2->radius);
}

template <>
bool shapes_Intersect<Circle, Point>(const Circle *hitbox1, const Point *hitbox2)
{
  return (hitbox1->position - hitbox2->position).mag() <= hitbox1->radius;
}

template <>
bool shapes_Intersect<Point, Point>(const Point *hitbox1, const Point *hitbox2)
{
  return hitbox1->position == hitbox2->position;
}

_GEAR_END