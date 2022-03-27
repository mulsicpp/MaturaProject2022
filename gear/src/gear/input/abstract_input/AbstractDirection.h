#pragma once

#include <gear/core/core.h>
#include <functional>
#include <gear/math/Vector.h>
#include "AbstractElement.h"

#define GEAR_DIRECTIONS(...) public: gear::Ref<gear::AbstractDirection> __VA_ARGS__;

#define GEAR_MAP_DIRECTION(name, type, ...) name = gear::Ref<gear::AbstractDirection>(new type(__VA_ARGS__));

_GEAR_START

class AbstractDirection : public AbstractElement<Vector<float, 2>> {
protected:
  bool m_Normalized;
public:
  virtual float get_X(void) = 0;
  virtual float get_Y(void) = 0;
  virtual Vector<float, 2> get_Direction(void) = 0;
  bool is_Normalized(void);
  void set_Normalized(bool normalized);
};

_GEAR_END