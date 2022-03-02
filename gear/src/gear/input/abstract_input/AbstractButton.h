#pragma once

#include <gear/core/core.h>
#include "../Input.h"

_GEAR_START

class AbstractButton {
protected:
  std::function<void(Action)> m_Callback;
public:
  virtual State get_State(void) = 0;
  void set_Callback(const std::function<void(Action)> &callback);
};

_GEAR_END