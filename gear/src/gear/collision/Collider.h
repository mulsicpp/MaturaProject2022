#pragma once

#include <gear/core/core.h>
#include "ExtendedShape.h"
#include <vector>

_GEAR_START

class Collider {
protected:
  std::vector<ExtendedShape> m_Shapes;
public:

  template<class T, class... Ts>
  static Ref<Collider> create(T shape, Ts... shapes)
  {
    Ref<Collider> ret(new Collider);
    ret->add(shape, shapes...);
    return ret;
  }

  template<class T>
  void add(T shape)
  {
    Ref<Shape> prev = Ref<Shape>(new T);
    prev->set_Enabled(false);
    m_Shapes.push_back({Ref<Shape>(new T(shape)), Ref<Shape>(new T(shape)), prev});
  }

  template<class T1, class T2, class... Ts>
  void add(T1 shape1, T2 shape2, Ts... shapes)
  {
    add<T1>(shape1);
    add<T2, Ts...>(shape2, shapes...);
  }

  std::vector<ExtendedShape> &get_Shapes(void);

  void transform(TransformComponent *transform);

  bool intersects(const Collider& other) const;
  bool intersected(const Collider& other) const;

  void set_Previous(void);

  void set_Enabled(bool enabled);
};

_GEAR_END