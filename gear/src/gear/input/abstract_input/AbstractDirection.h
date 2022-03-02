#pragma once

#include <gear/core/core.h>
#include <functional>
#include <gear/math/Vector.h>

_GEAR_START

class AbstractDirection {
protected:
  std::function<void(Vector<float, 2>)> m_Callback;
public:
  virtual float get_X(void) = 0;
  virtual float get_Y(void) = 0;
  virtual Vector<float, 2> get_Direction(void) = 0;
  void set_Callback(const std::function<void(Vector<float, 2>)> &callback);
};

_GEAR_END