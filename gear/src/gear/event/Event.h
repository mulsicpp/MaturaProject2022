#pragma once

#include <gear/core/core.h>
#include <gear/core/Window.h>

_GEAR_START

enum class EventType
{
    KEY_EVENT,
    TEXT_EVENT,
    MOUSE_MOVED_EVENT,
    MOUSE_BUTTON_EVENT,
    SCROLL_EVENT,
    WINDOW_FOCUS_EVENT,
    WINDOW_ICONIFY_EVENT,
    CONTROLLER_CONNECTION_EVENT,
    CONTROLLER_BUTTON_EVENT,
    CONTROLLER_AXIS_EVENT
};

enum class Action
{
    PRESSED = GLFW_PRESS,
    RELEASED = GLFW_RELEASE,
    REPEATED = GLFW_REPEAT
};

class Event
{
protected:
    const EventType m_Type;
    Event(EventType m_Type);
public:
    EventType getEventtype(void) const;
};

_GEAR_END