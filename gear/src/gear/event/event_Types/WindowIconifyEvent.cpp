#include "WindowIconifyEvent.h""

gear::WindowIconifyEvent::WindowIconifyEvent(int iconified) : Event(EventType::WINDOW_ICONIFY_EVENT), m_Iconified(iconified)
{
}

bool gear::WindowIconifyEvent::get_Iconified(void) const
{
    return m_Iconified;
}
