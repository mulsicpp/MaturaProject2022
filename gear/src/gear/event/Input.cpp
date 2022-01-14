#include "Input.h"
#include "Event.h"
#include "EventQueue.h"

#include <gear/core/debug/log.h>

GLFWwindow *gear::Input::m_Window = nullptr;

void gear::Input::init(void)
{
    GEAR_DEBUG_LOG("before current context");
    m_Window = glfwGetCurrentContext();
    GEAR_DEBUG_LOG("after current context");

}

gear::State gear::Input::get_Key_State(Key key)
{
    int state = glfwGetKey(m_Window, (int)key);
    return (State)state;
}

gear::Vector<double, 2> gear::Input::get_Cursor_Position()
{
    double xpos, ypos;
    glfwGetCursorPos(m_Window, &xpos, &ypos);
    return {xpos, ypos};
}

gear::State gear::Input::get_Mouse_Button_State(MouseButton mouse_Button)
{
    int state = glfwGetMouseButton(m_Window, (int)mouse_Button);
    return (State)state;
}
float gear::Input::get_Axis_Value(int controller_Id, ControllerAxis axis)
{
    // Implementation
    return 0;
}

gear::State gear::Input::get_Controller_Button_State(int controller_Id, ControllerButton controller_Button)
{
    // Implementation
    return State::PRESSED;
}

void gear::Input::dispatch_Events(Scene *scene){
    EventQueue<KeyEvent>::dispatch_Events(scene);
}
