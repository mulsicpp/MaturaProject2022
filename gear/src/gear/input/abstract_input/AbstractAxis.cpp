#include "AbstractAxis.h"

void gear::AbstractAxis::set_Callback(const std::function<void(float)> &callback)
{
  m_Callback = callback;
}