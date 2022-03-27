#include "AbstractButton.h"

gear::AbstractOrButton::AbstractOrButton(const std::vector<Ref<AbstractButton>> &buttons)
    : m_Buttons(buttons)
{
}

gear::State gear::AbstractOrButton::get_State(void) const
{
    for(const Ref<AbstractButton> &button : m_Buttons)
        if(button->get_State() == State::PRESSED)
            return State::PRESSED;
    return State::RELEASED;
}

void gear::AbstractOrButton::set_Callback(const std::function<void(Action)> &callback)
{
    m_Callback = callback;
    for(const Ref<AbstractButton> &button : m_Buttons) {
        button->set_Callback([this, button](Action a) {
            for(const Ref<AbstractButton> &b : m_Buttons)
                if(b != button && b->get_State() == State::PRESSED)
                    return;
            if(m_Callback != nullptr) m_Callback(a);
        });
    }
}