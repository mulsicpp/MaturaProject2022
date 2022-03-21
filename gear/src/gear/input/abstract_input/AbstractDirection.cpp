#include "AbstractDirection.h"

void gear::AbstractDirection::set_Callback(const std::function<void(gear::Vector<float, 2>)> &callback)
{
  m_Callback = callback;
}

  bool gear::AbstractDirection::is_Normalized(void){
    return m_Normalized;
  }
  void gear::AbstractDirection::set_Normalized(bool normalized){
    m_Normalized = normalized;
  }