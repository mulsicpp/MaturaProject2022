#include "KeyEvent.h"

gear::KeyEvent::KeyEvent(int key, int action) : Event(EventType::KEY_EVENT), m_Key((Key)key), m_Action((Action)action)  {
}
gear::Key gear::KeyEvent::get_Key(void) const {
    return m_Key;
}
gear::Action gear::KeyEvent::get_Action(void) const {
    return m_Action;
}