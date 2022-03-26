#pragma once

#include <gear/core/core.h>
#include "../InputDevice.h"

_GEAR_START

template<class I, class K, class C>
class InputSelector {
public:
  static Ref<I> create_From(InputDevice device) {
    if(device == InputDevice::KEYBOARD)
      return Ref<I>(new K);
    else
      return Ref<I>(new C((int)device - 1));
  }
};

_GEAR_END