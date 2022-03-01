#pragma once

#include <gear/core/core.h>
#include "../events/Event.h"
#include <gear/math/Vector.h>

_GEAR_START

class ScrollEvent : public Event
{
    friend class Game;

private:
    static void scroll_Event_Callback(GLFWwindow *window, double xoffset, double yoffset);
    Vector<double, 2> m_Offset;
    ScrollEvent(double x_Offset, double y_Offset);

public:
    double get_X_Offset(void) const;
    double get_Y_Offset(void) const;
    Vector<double, 2> get_Offset(void) const;
};
_GEAR_END