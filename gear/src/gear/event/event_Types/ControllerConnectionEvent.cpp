#include "ControllerConnectionEvent.h"

gear::ControllerConnectionEvent::ControllerConnectionEvent(int controller_Status) : Event(EventType::CONTROLLER_CONNECTION_EVENT), m_status((ControllerStatus)controller_Status)
{
}

gear::ControllerStatus gear::ControllerConnectionEvent::get_Controller_Status(void) const
{
    return m_status;
}