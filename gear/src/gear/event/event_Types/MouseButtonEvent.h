#pragma once

#include <gear/core/core.h>
#include "../Event.h"

_GEAR_START

enum class MouseButton
{
    LEFT = GLFW_MOUSE_BUTTON_1,
    RIGHT = GLFW_MOUSE_BUTTON_2,
    MIDDLE = GLFW_MOUSE_BUTTON_3,
    B_1 = GLFW_MOUSE_BUTTON_4,
    B_2 = GLFW_MOUSE_BUTTON_5,
    B_3 = GLFW_MOUSE_BUTTON_6,
    B_4 = GLFW_MOUSE_BUTTON_7,
    B_5 = GLFW_MOUSE_BUTTON_8,
};

class MouseButtonEvent : public Event
{
    friend class Game;

private:
    static void mouse_Button_Event_Callback(GLFWwindow *window, int button, int action, int mods);
    MouseButton m_Button;
    Action m_Action;
    MouseButtonEvent(int mouse_Button, int action);

public:
    MouseButton get_Mouse_Button(void) const;
    Action get_Action(void) const;
};
_GEAR_END