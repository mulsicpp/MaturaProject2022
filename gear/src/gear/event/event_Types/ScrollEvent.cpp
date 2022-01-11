#include "ScrollEvent.h"

gear::ScrollEvent::ScrollEvent(double x_Offset, double y_Offset) : Event(EventType::SCROLL_EVENT), m_Offset({x_Offset, y_Offset})
{
}

double gear::ScrollEvent::get_X_Offset(void) const
{
    return m_Offset[0];
}
double gear::ScrollEvent::get_Y_Offset(void) const
{
    return m_Offset[1];
}
gear::Vector<double, 2> gear::ScrollEvent::get_Offset(void) const
{
    return m_Offset;
}
