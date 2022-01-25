#include "ControllerButtonEvent.h"
#include "../EventQueue.h"

gear::ControllerButtonEvent::ControllerButtonEvent(int controller_Id, int button, int action) : Event(EventType::CONTROLLER_AXIS_EVENT), m_Controller_Id(controller_Id), m_Button((ControllerButton)button), m_Action((Action)action)
{
}

int gear::ControllerButtonEvent::get_Controller_Id(void) const
{
    return m_Controller_Id;
}
gear::ControllerButton gear::ControllerButtonEvent::get_Button(void) const
{
    return m_Button;
}

gear::Action gear::ControllerButtonEvent::get_Action(void) const
{
    return m_Action;
}