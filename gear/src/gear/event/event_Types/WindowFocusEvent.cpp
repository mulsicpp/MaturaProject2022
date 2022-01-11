#include "WindowFocusEvent.h"

gear::WindowFocusEvent::WindowFocusEvent(int focused) : Event(EventType::WINDOW_FOCUS_EVENT), m_Focused(focused)
{
}

bool gear::WindowFocusEvent::get_Focused(void) const
{
    return m_Focused;
}
