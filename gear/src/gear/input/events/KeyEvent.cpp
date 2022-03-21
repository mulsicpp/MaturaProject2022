#include "KeyEvent.h"
#include "../EventQueue.h"

void gear::KeyEvent::key_Event_Callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    gear::EventQueue<KeyEvent>::push_Event({key, action});
}

gear::KeyEvent::KeyEvent(int key, int action) : Event(EventType::KEYBOARD_EVENT), m_Key((Key)key), m_Action((Action)action)
{
}
gear::Key gear::KeyEvent::get_Key(void) const
{
    return m_Key;
}
gear::Action gear::KeyEvent::get_Action(void) const
{
    return m_Action;
}