#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

enum class ControllerButton
{
    A = GLFW_GAMEPAD_BUTTON_A,
    B = GLFW_GAMEPAD_BUTTON_B,
    X = GLFW_GAMEPAD_BUTTON_X,
    Y = GLFW_GAMEPAD_BUTTON_Y,
    LEFT_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
    RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
    BACK = GLFW_GAMEPAD_BUTTON_BACK,
    START = GLFW_GAMEPAD_BUTTON_START,
    GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE,
    LEFT_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
    RIGHT_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    DPAD_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP,
    DPAD_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    DPAD_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    DPAD_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
};

class Input;

class ControllerButtonEvent : public Event
{
    friend class Game;
    friend class Input;

private:
    int m_Controller_Id;
    ControllerButton m_Button;
    Action m_Action;
    ControllerButtonEvent(int controller_Id, int button, int action);

public:
    int get_Controller_Id(void) const;
    ControllerButton get_Button(void) const;
    Action get_Action(void) const;
};
_GEAR_END