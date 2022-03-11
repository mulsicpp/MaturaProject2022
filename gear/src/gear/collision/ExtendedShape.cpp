#include "ExtendedShape.h"

gear::ExtendedShape::ExtendedShape(Ref<Shape> shape, Ref<Shape> absolute_Shape, Ref<Shape> previous_Shape)
    : shape(shape), absolute_Shape(absolute_Shape), previous_Shape(previous_Shape)
{
}

gear::ExtendedShape::ExtendedShape(const ExtendedShape &shape) {
  this->shape = shape.shape->copy();
  this->absolute_Shape = shape.absolute_Shape->copy();
  this->previous_Shape = shape.previous_Shape->copy();
}

gear::ExtendedShape &gear::ExtendedShape::operator=(const ExtendedShape &shape) {
  this->shape = shape.shape->copy();
  this->absolute_Shape = shape.absolute_Shape->copy();
  this->previous_Shape = shape.previous_Shape->copy();

  return *this;
}