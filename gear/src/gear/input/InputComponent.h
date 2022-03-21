#pragma once

#include <gear/core/core.h>
#include <functional>

_GEAR_START

template <class T>
struct InputComponent
{
    std::function<void(T)> callback;
    bool active;
    InputComponent(void) = default;
    InputComponent(std::function<void(T)> callback, bool active = true) : callback(callback), active(active) {}
};

_GEAR_END