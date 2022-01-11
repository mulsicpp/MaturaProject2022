#include "MouseMovedEvent.h"

gear::MouseMovedEvent::MouseMovedEvent(double x, double y) : Event(EventType::MOUSE_MOVED_EVENT), m_Pos({x, y}) {}

gear::Vector<double, 2> gear::MouseMovedEvent::get_Pos(void) const
{
    return m_Pos;
}

double gear::MouseMovedEvent::get_X(void) const
{
    return m_Pos[0];
}
double gear::MouseMovedEvent::get_Y(void) const
{
    return  m_Pos[1];
}