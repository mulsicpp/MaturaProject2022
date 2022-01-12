#pragma once

#include <gear/core/core.h>
#include <functional>

_GEAR_START

template <class T>
struct EventComponent
{
    std::function<void(T)> callback;
    bool active;
    EventComponent(void) = default;
    EventComponent(std::function<void(T)> callback, bool active = true) : callback(callback), active(active) {}
};

_GEAR_END