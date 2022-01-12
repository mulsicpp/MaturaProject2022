#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

class WindowFocusEvent : public Event
{
    friend class Game;

private:
    static void window_Focus_Event_Callback(GLFWwindow *window, int focused);
    bool m_Focused;
    WindowFocusEvent(int focused);

public:
    bool get_Focused(void) const;
};
_GEAR_END