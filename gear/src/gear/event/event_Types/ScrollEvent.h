#pragma once

#include <gear/core/core.h>
#include "../Event.h"
#include <gear/math/Vector.h>

_GEAR_START

class ScrollEvent : public Event
{
private:
    Vector<double, 2> m_Offset;
    ScrollEvent(double x_Offset, double y_Offset);

public:
    double get_X_Offset(void) const;
    double get_Y_Offset(void) const;
    Vector<double, 2> get_Offset(void) const;
};
_GEAR_END