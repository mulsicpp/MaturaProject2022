#include "AKeyboardButton.h"

gear::AKeyboardButton::AKeyboardButton(gear::Key button) : m_Button(button) {}

gear::State gear::AKeyboardButton::get_State(void) const
{
  return Input::get_Key_State(m_Button);
}

void gear::AKeyboardButton::handle_Event(gear::KeyEvent event) {
  if(event.get_Key() == m_Button && m_Callback != nullptr) {
    if(event.get_Action() == Action::PRESSED)
        m_Callback(Action::PRESSED);
    else if(event.get_Action() == Action::RELEASED)
        m_Callback(Action::RELEASED);
  }
}