#include "AControllerButton.h"

gear::AControllerButton::AControllerButton(int controller_ID, gear::ControllerButton button) : m_Controller_ID(controller_ID), m_Button(button) {}

gear::State gear::AControllerButton::get_State(void) const
{
  return m_Enabled ? Input::get_Controller_Button_State(m_Controller_ID, m_Button) : State::RELEASED;
}

void gear::AControllerButton::handle_Event(gear::ControllerButtonEvent event) {
  if(m_Enabled && event.get_Controller_Id() == m_Controller_ID && event.get_Button() == m_Button && m_Callback != nullptr) {
    m_Callback(event.get_Action());
  }
}