#include "AbstractElement.h"

void gear::AbstractElement::set_Callback(const std::function<void(Action)> &callback)
{
  m_Callback = callback;
}

std::function<void(gear::Action)> gear::AbstractElement::get_Callback(void) const {
    return m_Callback;
}