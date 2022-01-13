#include "ControllerAxisEvent.h"

gear::ControllerAxisEvent::ControllerAxisEvent(int controller_Id, int axis, float value) : Event(EventType::CONTROLLER_AXIS_EVENT), m_Controller_Id(controller_Id), m_Axis((ControllerAxis)axis), m_Value(value) {}

gear::ControllerAxis gear::ControllerAxisEvent::get_Axis(void) const
{
    return m_Axis;
}
float gear::ControllerAxisEvent::get_Value(void) const
{
    return m_Value;
}
int gear::ControllerAxisEvent::get_Controller_Id(void) const
{
    return m_Controller_Id;
}