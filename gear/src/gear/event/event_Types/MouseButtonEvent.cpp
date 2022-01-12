#include "MouseButtonEvent.h"
#include "../EventQueue.h"


void gear::MouseButtonEvent::mouse_Button_Event_Callback(GLFWwindow* window, int button, int action, int mods)
{
    gear::EventQueue<MouseButtonEvent>::push_Event({button, action});
}

gear::MouseButtonEvent::MouseButtonEvent(int mouse_Button, int action) : Event(EventType::MOUSE_BUTTON_EVENT), m_Button((MouseButton)mouse_Button), m_Action((Action)action)
{
}

gear::MouseButton gear::MouseButtonEvent::get_Mouse_Button(void) const
{
    return m_Button;
}
gear::Action gear::MouseButtonEvent::get_Action(void) const
{
    return m_Action;
}