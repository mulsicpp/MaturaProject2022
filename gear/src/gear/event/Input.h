#pragma once

#include <gear/core/core.h>
#include <vector>
#include <functional>
#include "event_Types/KeyEvent.h"
#include <gear/math/Vector.h>
#include "event_Types/MouseButtonEvent.h"
#include "event_Types/ControllerAxisEvent.h"
#include "event_Types/ControllerButtonEvent.h"
#include "event_Types/ControllerConnectionEvent.h"
#include <gear/scene/Scene.h>
#include "EventQueue.h"
#include <thread>
#include <unordered_map>

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
    static bool m_Controller_Thread_Running;
    static std::thread m_Controller_Thread;
    static std::unordered_map<int, GLFWgamepadstate> m_Controller_Map;
    static void controller_Loop(void);
    static void update_Controllermap(void);

public:
    template <class T>
    static void add_Global_Callback(std::function<void(T)> callback)
    {
        EventQueue<T>::m_Global_Event_Callbacks.push_back(callback);
    }
    static void init(void);
    static State get_Key_State(Key key);
    static gear::Vector<double, 2> get_Cursor_Position();
    static State get_Mouse_Button_State(MouseButton mouse_Button);
    static float get_Axis_Value(int controller_Id, ControllerAxis axis);
    static State get_Controller_Button_State(int controller_Id, ControllerButton controller_Button);
    static void dispatch_Events(Scene *scene);
    static void destroy(void);
};

_GEAR_END