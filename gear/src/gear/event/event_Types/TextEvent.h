#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

class TextEvent : public Event
{
private:
    unsigned int m_Unicode_Value;
    TextEvent(unsigned int unicode_Value);

public:
    unsigned int get_Unicode_Value(void) const;
};
_GEAR_END