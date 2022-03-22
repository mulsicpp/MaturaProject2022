#include "AControllerAxisButton.h"

gear::AControllerAxisButton::AControllerAxisButton(int controller_ID, ControllerAxis button, float intervall_Min, float intervall_Max)
{
    m_Controller_ID = controller_ID;
    m_Button = button;
    m_Intervall_Min = intervall_Min;
    m_Intervall_Max = intervall_Max;
}

gear::State gear::AControllerAxisButton::get_State(void) const
{
    return m_State;
}

void gear::AControllerAxisButton::handle_Event(gear::ControllerAxisEvent event)
{
    if (event.get_Controller_Id() == m_Controller_ID && event.get_Axis() == m_Button)
        if (Input::get_Axis_Value(m_Controller_ID, m_Button) <= m_Intervall_Max && Input::get_Axis_Value(m_Controller_ID, m_Button) >= m_Intervall_Min)
        {
            if (m_State == State::RELEASED && m_Callback != nullptr)
                m_Callback(Action::PRESSED);
            m_State = State::PRESSED;
        }
        else
        {
            if (m_State == State::PRESSED && m_Callback != nullptr)
                m_Callback(Action::RELEASED);
            m_State = State::RELEASED;
        }
}

void gear::AControllerAxisButton::set_Intervall(float intervall_Min, float intervall_Max)
{
    m_Intervall_Min = intervall_Min;
    m_Intervall_Max = intervall_Max;
}
float gear::AControllerAxisButton::get_Intervall_Min() const
{
    return m_Intervall_Min;
}
float gear::AControllerAxisButton::get_Intervall_Max() const
{
    return m_Intervall_Max;
}