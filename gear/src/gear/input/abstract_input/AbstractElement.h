#pragma once

#include <gear/core/core.h>
#include <functional>
#include "../Input.h"

_GEAR_START

template<class T>
class AbstractElement {
protected:
  std::function<void(T)> m_Callback;
  bool m_Enabled = true;
public:
  virtual void set_Callback(const std::function<void(T)> &callback) { m_Callback = callback; }
  virtual std::function<void(T)> get_Callback(void) const { return m_Callback; }

  virtual void set_Enabled(bool enabled) { m_Enabled = enabled; }
  virtual bool is_Enabled(void) const { return m_Enabled; }
};

_GEAR_END