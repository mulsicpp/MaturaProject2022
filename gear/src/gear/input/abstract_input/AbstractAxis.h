#pragma once

#include <gear/core/core.h>
#include <functional>

#define GEAR_AXES(...) public: gear::Ref<gear::AbstractAxis> __VA_ARGS__;

#define GEAR_MAP_AXIS(name, type, ...) name = gear::Ref<gear::AbstractAxis>(new type(__VA_ARGS__));

_GEAR_START

class AbstractAxis {
protected:
  std::function<void(float)> m_Callback;
public:
  virtual float get_Value(void) = 0;
  void set_Callback(const std::function<void(float)> &callback);
};

_GEAR_END