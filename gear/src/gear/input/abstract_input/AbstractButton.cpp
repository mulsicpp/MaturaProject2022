#include "AbstractButton.h"

void gear::AbstractButton::set_Callback(const std::function<void(Action)> &callback)
{
    m_Callback = callback;
}

gear::AbstractOrButton::AbstractOrButton(const Ref<AbstractButton> &first, const Ref<AbstractButton> &second)
    : m_First(first), m_Second(second)
{
}

gear::State gear::AbstractOrButton::get_State(void) const
{
    return m_First->get_State() == State::PRESSED ? State::PRESSED : m_Second->get_State();
}

void gear::AbstractOrButton::set_Callback(const std::function<void(Action)> &callback)
{
    m_First->set_Callback(callback);
    m_Second->set_Callback(callback);
}