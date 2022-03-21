#pragma once

#include <gear/core/core.h>
#include "../Input.h"

#define GEAR_BUTTONS(...) public: gear::Ref<gear::AbstractButton> __VA_ARGS__;

#define GEAR_MAP_BUTTON(name, type, ...) name = gear::Ref<gear::AbstractButton>(new type(__VA_ARGS__));

_GEAR_START

class AbstractButton {
protected:
  std::function<void(Action)> m_Callback;
public:
  virtual State get_State(void) const = 0;
  void set_Callback(const std::function<void(Action)> &callback);
};

_GEAR_END