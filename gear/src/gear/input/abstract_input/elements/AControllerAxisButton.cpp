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
            m_State = State::PRESSED;
            m_Callback(Action::PRESSED);
        }
        else
        {
            m_State = State::RELEASED;
            m_Callback(Action::RELEASED);
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