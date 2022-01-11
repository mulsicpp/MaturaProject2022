#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

class WindowFocusEvent : public Event
{
private:
    bool m_Focused;
    WindowFocusEvent(int focused);

public:
    bool get_Focused(void) const;
};
_GEAR_END