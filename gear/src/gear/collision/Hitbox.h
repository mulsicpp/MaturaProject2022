#pragma once

#include <gear/core/core.h>
#include "shapes/Shape.h"

_GEAR_START

class Hitbox {
public:
  Ref<Shape> m_Shape;
  Ref<Shape> m_Absolute_Shape;
  Ref<Shape> m_Previous_Shape;

  template<class T>
  static Hitbox create(T shape)
  {
    Hitbox ret;
    ret.m_Shape = Ref<Shape>(new T(shape));
    ret.m_Absolute_Shape = Ref<Shape>(new T(shape));
    ret.m_Previous_Shape = Ref<Shape>(new T(shape));

    return ret;
  }

  void transform(TransformComponent *transform);
};

_GEAR_END