#include "Shape.h"
#include "Rect.h"
#include "Circle.h"
#include "Point.h"

#include <gear/core/debug/log.h>

_GEAR_START

template <class T1, class T2>
bool shapes_Intersect(const T1 *hitbox1, const T2 *hitbox2, Vector<double, 2> *separation_Vector)
{
  gear::error("A parameter is not a shape");
  return false;
}

template <>
bool shapes_Intersect<Rect, Rect>(const Rect *hitbox1, const Rect *hitbox2, Vector<double, 2> *separation_Vector)
{
  bool ret = !(hitbox2->top_Left[0] >= hitbox1->bottom_Right[0] ||
               hitbox2->bottom_Right[0] <= hitbox1->top_Left[0] ||
               hitbox2->top_Left[1] >= hitbox1->bottom_Right[1] ||
               hitbox2->bottom_Right[1] <= hitbox1->top_Left[1]);

  if (ret && separation_Vector)
  {
    double distance = 0;
    double min_Dist = INFINITY;
    short direction = 0;

    distance = hitbox1->bottom_Right[0] - hitbox2->top_Left[0];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 1;
    }

    distance = hitbox2->bottom_Right[0] - hitbox1->top_Left[0];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 2;
    }

    distance = hitbox1->bottom_Right[1] - hitbox2->top_Left[1];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 3;
    }

    distance = hitbox2->bottom_Right[1] - hitbox1->top_Left[1];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 4;
    }

    switch (direction)
    {
    case 0:
      *separation_Vector = {0, 0};
      break;
    case 1:
      *separation_Vector = {min_Dist, 0};
      break;
    case 2:
      *separation_Vector = {-min_Dist, 0};
      break;
    case 3:
      *separation_Vector = {0, min_Dist};
      break;
    case 4:
      *separation_Vector = {0, -min_Dist};
      break;
    }
  }

  return ret;
}

template <>
bool shapes_Intersect<Rect, Circle>(const Rect *hitbox1, const Circle *hitbox2, Vector<double, 2> *separation_Vector)
{
  double r_Left = hitbox1->top_Left[0],
         r_Right = hitbox1->bottom_Right[0],
         r_Top = hitbox1->top_Left[1],
         r_Bottom = hitbox1->bottom_Right[1],
         c_X = hitbox2->position[0],
         c_Y = hitbox2->position[1],
         c_Rad = hitbox2->radius;

  bool ret = false;

  if (c_X > r_Left - c_Rad && c_X < r_Right + c_Rad && c_Y > r_Top && c_Y < r_Bottom)
    ret = true;
  if (c_X > r_Left && c_X < r_Right && c_Y > r_Top - c_Rad && c_Y < r_Bottom + c_Rad)
    ret = true;

  if (ret && separation_Vector)
  {
    double distance = 0;
    double min_Dist = INFINITY;
    short direction = 0;

    distance = r_Right - c_X + c_Rad;
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 1;
    }

    distance = c_X - r_Left + c_Rad;
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 2;
    }

    distance = r_Bottom - c_Y + c_Rad;
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 3;
    }

    distance = c_Y - r_Top + c_Rad;
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 4;
    }

    switch (direction)
    {
    case 0:
      *separation_Vector = {0, 0};
      break;
    case 1:
      *separation_Vector = {min_Dist, 0};
      break;
    case 2:
      *separation_Vector = {-min_Dist, 0};
      break;
    case 3:
      *separation_Vector = {0, min_Dist};
      break;
    case 4:
      *separation_Vector = {0, -min_Dist};
      break;
    }
  }

  if (ret)
    return true;

  Vector<double, 2> vec;
  double dist;

  double root_Of_Two = 1 / sqrt(2);

  vec = hitbox2->position - hitbox1->top_Left;
  dist = vec.mag();
  if (dist < c_Rad)
  {
    if (separation_Vector)
    {
      if (dist <= c_Rad / 10000)
        *separation_Vector = {-root_Of_Two * c_Rad, -root_Of_Two * c_Rad};
      else
        *separation_Vector = vec * (c_Rad / dist) - vec;
    }
    return true;
  }

  vec = hitbox2->position - hitbox1->bottom_Right;
  dist = vec.mag();
  if (dist < c_Rad)
  {
    if (separation_Vector)
    {
      if (dist <= c_Rad / 10000)
        *separation_Vector = {root_Of_Two * c_Rad, root_Of_Two * c_Rad};
      else
        *separation_Vector = vec * (c_Rad / dist) - vec;
    }
    return true;
  }

  vec = hitbox2->position - Vector<double, 2>{r_Right, r_Top};
  dist = vec.mag();
  if (dist < c_Rad)
  {
    if (separation_Vector)
    {
      if (dist <= c_Rad / 10000)
        *separation_Vector = {root_Of_Two * c_Rad, -root_Of_Two * c_Rad};
      else
        *separation_Vector = vec * (c_Rad / dist) - vec;
    }
    return true;
  }

  vec = hitbox2->position - Vector<double, 2>{r_Left, r_Bottom};
  dist = vec.mag();
  if (dist < c_Rad)
  {
    if (separation_Vector)
    {
      if (dist <= c_Rad / 10000)
        *separation_Vector = {-root_Of_Two * c_Rad, root_Of_Two * c_Rad};
      else
        *separation_Vector = vec * (c_Rad / dist) - vec;
    }
    return true;
  }

  return false;
}

template <>
bool shapes_Intersect<Rect, Point>(const Rect *hitbox1, const Point *hitbox2, Vector<double, 2> *separation_Vector)
{
  bool ret = (hitbox1->top_Left[0] < hitbox2->position[0] &&
              hitbox1->bottom_Right[0] > hitbox2->position[0] &&
              hitbox1->top_Left[1] < hitbox2->position[1] &&
              hitbox1->bottom_Right[1] > hitbox2->position[1]);

  if (ret && separation_Vector)
  {
    double distance = 0;
    double min_Dist = INFINITY;
    short direction = 0;

    distance = hitbox1->bottom_Right[0] - hitbox2->position[0];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 1;
    }

    distance = hitbox2->position[0] - hitbox1->top_Left[0];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 2;
    }

    distance = hitbox1->bottom_Right[1] - hitbox2->position[1];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 3;
    }

    distance = hitbox2->position[1] - hitbox1->top_Left[1];
    if (distance > 0 && distance < min_Dist)
    {
      min_Dist = distance;
      direction = 4;
    }

    switch (direction)
    {
    case 0:
      *separation_Vector = {0, 0};
      break;
    case 1:
      *separation_Vector = {min_Dist, 0};
      break;
    case 2:
      *separation_Vector = {-min_Dist, 0};
      break;
    case 3:
      *separation_Vector = {0, min_Dist};
      break;
    case 4:
      *separation_Vector = {0, -min_Dist};
      break;
    }
  }

  return ret;
}

template <>
bool shapes_Intersect<Circle, Circle>(const Circle *hitbox1, const Circle *hitbox2, Vector<double, 2> *separation_Vector)
{
  auto vec = hitbox2->position - hitbox1->position;
  double dist = vec.mag();
  if (separation_Vector)
  {
    if (dist <= hitbox1->radius + hitbox2->radius / 10000)
      *separation_Vector = {hitbox1->radius + hitbox2->radius, 0};
    else
      *separation_Vector = vec * ((hitbox1->radius + hitbox2->radius) / dist) - vec;
  }
  return dist < hitbox1->radius + hitbox2->radius;
}

template <>
bool shapes_Intersect<Circle, Point>(const Circle *hitbox1, const Point *hitbox2, Vector<double, 2> *separation_Vector)
{
  auto vec = hitbox2->position - hitbox1->position;
  double dist = vec.mag();
  if (separation_Vector)
  {
    if (dist <= hitbox1->radius / 10000)
      *separation_Vector = {hitbox1->radius, 0};
    else
      *separation_Vector = vec * (hitbox1->radius / dist) - vec;
  }
  return dist < hitbox1->radius;
}

template <>
bool shapes_Intersect<Point, Point>(const Point *hitbox1, const Point *hitbox2, Vector<double, 2> *separation_Vector)
{
  return hitbox1->position == hitbox2->position;
}

_GEAR_END