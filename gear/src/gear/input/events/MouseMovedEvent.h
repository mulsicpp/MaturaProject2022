#pragma once

#include <gear/core/core.h>
#include "../events/Event.h"
#include <gear/math/Vector.h>

_GEAR_START

class MouseMovedEvent : public Event
{
    friend class Game;

private:
    static void mouse_Moved_Event_Callback(GLFWwindow *window, double xpos, double ypos);
    Vector<double, 2> m_Pos;
    MouseMovedEvent(double x, double y);

public:
    double get_X(void) const;
    double get_Y(void) const;
    Vector<double, 2> get_Pos(void) const;
};
_GEAR_END