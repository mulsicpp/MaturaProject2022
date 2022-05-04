#include "AMouseButton.h"

gear::AMouseButton::AMouseButton(gear::MouseButton button) : m_Button(button) {}

gear::State gear::AMouseButton::get_State(void) const
{
  return m_Enabled ? Input::get_Mouse_Button_State(m_Button) : State::RELEASED;
}

void gear::AMouseButton::handle_Event(gear::MouseButtonEvent event) {
  if(m_Enabled && event.get_Mouse_Button() == m_Button && m_Callback != nullptr) {
    m_Callback(event.get_Action());
  }
}