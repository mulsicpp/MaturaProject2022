#include "AbstractButton.h"

void gear::AbstractButton::set_Callback(const std::function<void(Action)> &callback)
{
  m_Callback = callback;
}