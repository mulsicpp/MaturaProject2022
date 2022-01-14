#pragma once

#include <gear/core/core.h>
#include <vector>
#include <functional>
#include "event_Types/KeyEvent.h"
#include <gear/math/Vector.h>
#include "event_Types/MouseButtonEvent.h"
#include "event_Types/ControllerAxisEvent.h"
#include "event_Types/ControllerButtonEvent.h"
#include <gear/scene/Scene.h>

_GEAR_START

enum class State
{
    PRESSED = GLFW_PRESS,
    RELEASED = GLFW_RELEASE
};

class Input
{
private:
    static GLFWwindow *m_Window;

public:
    template <class T>
    static void add_Global_Callback(std::function<void(T)> callback)
    {
        EventQueue<T>::global_Event_Callbacks.push_back(callback);
    }
    static void init(void);
    static State get_Key_State(Key key);
    static gear::Vector<double, 2> get_Cursor_Position();
    static State get_Mouse_Button_State(MouseButton mouse_Button);
    static float get_Axis_Value(int controller_Id, ControllerAxis axis);
    static State get_Controller_Button_State(int controller_Id, ControllerButton controller_Button);
    static void dispatch_Events(Scene *scene);
};

_GEAR_END