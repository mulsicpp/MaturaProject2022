#include "TextEvent.h"
#include "../EventQueue.h"


void gear::TextEvent::text_Event_Callback(GLFWwindow* window, unsigned int codepoint)
{
    gear::EventQueue<TextEvent>::push_Event({codepoint});
}

gear::TextEvent::TextEvent(unsigned int unicode_Value) : Event(EventType::TEXT_EVENT), m_Unicode_Value(unicode_Value){}

 unsigned int gear::TextEvent::get_Unicode_Value(void) const{
     return m_Unicode_Value;
 }

