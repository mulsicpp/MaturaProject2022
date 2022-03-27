#pragma once

#include <gear/core/core.h>
#include <functional>
#include "../Input.h"

_GEAR_START

template<class T>
class AbstractElement {
protected:
  std::function<void(T)> m_Callback;
public:
  virtual void set_Callback(const std::function<void(T)> &callback) { m_Callback = callback; }
  virtual std::function<void(T)> get_Callback(void) const { return m_Callback; }
};

_GEAR_END