#include "AbstractDirection.h"

void gear::AbstractDirection::set_Callback(const std::function<void(gear::Vector<float, 2>)> &callback)
{
  m_Callback = callback;
}