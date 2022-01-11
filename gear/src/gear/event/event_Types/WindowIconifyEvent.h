#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

class WindowIconifyEvent : public Event
{
private:
    bool m_Iconified;
    WindowIconifyEvent(int iconified);

public:
    bool get_Iconified(void) const;
};
_GEAR_END