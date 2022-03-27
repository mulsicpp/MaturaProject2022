#pragma once

#include <gear/core/core.h>
#include <functional>
#include "../Input.h"

_GEAR_START

class AbstractElement {
protected:
  std::function<void(Action)> m_Callback;
public:
  void set_Callback(const std::function<void(Action)> &callback);
  std::function<void(Action)> get_Callback(void) const;
};

_GEAR_END