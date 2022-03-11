#include "Rect.h"

#include <gear/core/debug/log.h>

gear::Rect::Rect(void) : Rect({0, 0}, {1, 1}) {}

gear::Rect::Rect(const gear::Vector<double, 2> &top_Left, const gear::Vector<double, 2> &bottom_Right, bool enabled)
    : Shape(ShapeType::RECT), top_Left(top_Left), bottom_Right(bottom_Right)
{
  set_Enabled(enabled);
}

void gear::Rect::transform(const gear::TransformComponent *transform, gear::Shape *transformed_Shape) const
{
  Rect *s = (Rect *)transformed_Shape;
  s->top_Left = (transform->get_Matrix() * Vector<double, 3>{top_Left[0], top_Left[1], 1}).use_As<2>();
  s->bottom_Right = (transform->get_Matrix() * Vector<double, 3>{bottom_Right[0], bottom_Right[1], 1}).use_As<2>();

  if (s->top_Left[0] > s->bottom_Right[0])
    GEAR_SWAP_VALUES(s->top_Left[0], s->bottom_Right[0]);

  if (s->top_Left[1] > s->bottom_Right[1])
    GEAR_SWAP_VALUES(s->top_Left[1], s->bottom_Right[1]);
}

void gear::Rect::assign(const gear::Shape *other)
{
  Shape::assign(other);
  if(get_Type() == other->get_Type()) {
    this->top_Left = ((Rect *)other)->top_Left;
    this->bottom_Right = ((Rect *)other)->bottom_Right;
  }
}

gear::Ref<gear::Shape> gear::Rect::copy(void) const {
  return Ref<Rect>(new Rect(*this));
}