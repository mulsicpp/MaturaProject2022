#include "MouseButtonEvent.h"

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