#include "AControllerAxis.h"
#include "../../Input.h"

gear::AControllerAxis::AControllerAxis(int controller_ID, ControllerAxis axis)
{
    m_Controller_ID = controller_ID;
    m_Axis = axis;
}
float gear::AControllerAxis::get_Value(void) const
{
    return Input::get_Axis_Value(m_Controller_ID, m_Axis);
}
void gear::AControllerAxis::handle_Event(ControllerAxisEvent event)
{
    if (event.get_Controller_Id() == m_Controller_ID && event.get_Axis() == m_Axis)
    {
        m_Callback(event.get_Value());
    }
}