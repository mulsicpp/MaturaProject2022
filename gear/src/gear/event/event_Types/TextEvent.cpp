#include "TextEvent.h"

gear::TextEvent::TextEvent(unsigned int unicode_Value) : Event(EventType::TEXT_EVENT), m_Unicode_Value(unicode_Value){}

 unsigned int gear::TextEvent::get_Unicode_Value(void) const{
     return m_Unicode_Value;
 }

