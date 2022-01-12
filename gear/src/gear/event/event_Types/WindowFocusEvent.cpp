#include "WindowFocusEvent.h"
#include "../EventQueue.h"


void gear::WindowFocusEvent::window_Focus_Event_Callback(GLFWwindow* window, int focused)
{
    gear::EventQueue<WindowFocusEvent>::push_Event({focused});
}

gear::WindowFocusEvent::WindowFocusEvent(int focused) : Event(EventType::WINDOW_FOCUS_EVENT), m_Focused(focused)
{
}

bool gear::WindowFocusEvent::get_Focused(void) const
{
    return m_Focused;
}
