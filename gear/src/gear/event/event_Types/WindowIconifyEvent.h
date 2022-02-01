#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

class WindowIconifyEvent : public Event
{
    friend class Game;

private:
    static void window_Iconify_Event_Callback(GLFWwindow *window, int iconified);
    bool m_Iconified;
    WindowIconifyEvent(int iconified);

public:
    bool get_Iconified(void) const;
};
_GEAR_END