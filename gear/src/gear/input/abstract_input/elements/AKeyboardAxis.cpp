#include "AKeyboardAxis.h"
#include "../../Input.h"

gear::AKeyboardAxis::AKeyboardAxis(Key min_Key, Key max_Key)
{
    m_Max_Key = max_Key;
    m_Min_Key = min_Key;
}
float gear::AKeyboardAxis::get_Value(void) const
{
    float ret = 0;
    if (Input::get_Key_State(m_Max_Key) == State::PRESSED)
        ret++;
    if (Input::get_Key_State(m_Min_Key) == State::PRESSED)
        ret--;
    return ret;
}
void gear::AKeyboardAxis::handle_Event(KeyEvent event)
{
    float value = 0;
    if (event.get_Key() == m_Max_Key)
    {
        if (event.get_Action() == Action::PRESSED)
            value++;
        if (Input::get_Key_State(m_Min_Key) == State::PRESSED)
            value--;
    }
    else if (event.get_Key() == m_Min_Key)
    {
        if (event.get_Action() == Action::PRESSED)
            value--;
        if (Input::get_Key_State(m_Max_Key) == State::PRESSED)
            value++;
    }
    if (m_Callback != nullptr)
        m_Callback(value);
}