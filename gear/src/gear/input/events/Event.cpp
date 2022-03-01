#include "Event.h"

gear::Event::Event(gear::EventType m_Type) : m_Type(m_Type) {}

gear::EventType gear::Event::getEventtype(void) const{
    return m_Type;
}