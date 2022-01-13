#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

enum class ControllerAxis{
LEFT_STICK_X = GLFW_GAMEPAD_AXIS_LEFT_X,
LEFT_STICK_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
RIGHT_STICK_X = GLFW_GAMEPAD_AXIS_RIGHT_X,
RIGHT_STICK_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,
LEFT_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
};

class ControllerAxisEvent : public Event
{
    friend class Game;

private:
    int m_Controller_Id;
    ControllerAxis m_Axis;
    float m_Value;
    ControllerAxisEvent(int controller_ID, int axis, float value);

public:
    ControllerAxis get_Axis(void) const;
    float get_Value(void) const;
    int get_Controller_Id(void) const;

};
_GEAR_END