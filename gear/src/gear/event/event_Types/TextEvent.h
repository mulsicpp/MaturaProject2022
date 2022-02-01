#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

class TextEvent : public Event
{
    friend class Game;

private:
    static void text_Event_Callback(GLFWwindow *window, unsigned int codepoint);
    unsigned int m_Unicode_Value;
    TextEvent(unsigned int unicode_Value);

public:
    unsigned int get_Unicode_Value(void) const;
};
_GEAR_END