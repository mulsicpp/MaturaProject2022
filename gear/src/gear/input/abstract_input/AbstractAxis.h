#pragma once

#include <gear/core/core.h>
#include <functional>

_GEAR_START

class AbstractAxis {
protected:
  std::function<void(float)> m_Callback;
public:
  virtual float get_Value(void) = 0;
  void set_Callback(const std::function<void(float)> &callback);
};

_GEAR_END