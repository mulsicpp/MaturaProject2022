#include "Event.h"

gear::Event::Event(gear::EventType m_Type) : m_Type(m_Type) {}

gear::EventType gear::Event::get_Eventtype(void) const
{
  return m_Type;
}