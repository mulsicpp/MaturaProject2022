#include "abstract_input.h"

gear::AbstractKeyboardInput::AbstractKeyboardInput(void) {}

gear::AbstractControllerInput::AbstractControllerInput(int id) : m_ID(id) {}

int gear::AbstractControllerInput::get_ID() {
  return m_ID;
}