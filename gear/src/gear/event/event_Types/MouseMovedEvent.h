#pragma once

#include <gear/core/core.h>
#include "../Event.h"
#include <gear/math/Vector.h>

_GEAR_START

class MouseMovedEvent : public Event
{
private:
    Vector<double, 2> m_Pos;
    MouseMovedEvent(double x, double y);

public:
    double get_X(void) const;
    double get_Y(void) const;
    Vector<double, 2> get_Pos(void) const;
};
_GEAR_END