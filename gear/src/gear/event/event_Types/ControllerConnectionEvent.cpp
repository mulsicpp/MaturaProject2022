#include "ControllerConnectionEvent.h"
#include "../EventQueue.h"

void gear::ControllerConnectionEvent::controller_Connection_Event_Callback(int jid, int event)
{
    gear::EventQueue<ControllerConnectionEvent>::push_Event({jid, event});
}

gear::ControllerConnectionEvent::ControllerConnectionEvent(int controller_ID, int controller_Status) : Event(EventType::CONTROLLER_CONNECTION_EVENT), m_status((ControllerStatus)controller_Status), m_Controller_Id(controller_ID)
{
}

gear::ControllerStatus gear::ControllerConnectionEvent::get_Controller_Status(void) const
{
    return m_status;
}