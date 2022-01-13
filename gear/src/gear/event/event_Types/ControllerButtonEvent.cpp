#include "ControllerButtonEvent.h"

gear::ControllerButtonEvent::ControllerButtonEvent(int controller_Id, int button) : Event(EventType::CONTROLLER_AXIS_EVENT), m_Controller_Id(controller_Id), m_Button((ControllerButton)button) {}

 int gear::ControllerButtonEvent::get_Controller_Id(void) const{
     return m_Controller_Id;
 }
    gear::ControllerButton gear::ControllerButtonEvent::get_Button(void) const{
        return m_Button;
    }