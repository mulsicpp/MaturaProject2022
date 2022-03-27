#pragma once

#include <gear/core/core.h>
#include <functional>

#include "AbstractElement.h"

#define GEAR_AXES(...) public: gear::Ref<gear::AbstractAxis> __VA_ARGS__;

#define GEAR_MAP_AXIS(name, type, ...) name = gear::Ref<gear::AbstractAxis>(new type(__VA_ARGS__));

_GEAR_START

class AbstractAxis : public AbstractElement<float> {
public:
  virtual float get_Value(void) const = 0;
};

_GEAR_END