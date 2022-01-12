#include "WindowIconifyEvent.h"
#include "../EventQueue.h"


void gear::WindowIconifyEvent::window_Iconify_Event_Callback(GLFWwindow* window, int iconified)
{
    gear::EventQueue<WindowIconifyEvent>::push_Event({iconified});
}

gear::WindowIconifyEvent::WindowIconifyEvent(int iconified) : Event(EventType::WINDOW_ICONIFY_EVENT), m_Iconified(iconified)
{
}

bool gear::WindowIconifyEvent::get_Iconified(void) const
{
    return m_Iconified;
}
