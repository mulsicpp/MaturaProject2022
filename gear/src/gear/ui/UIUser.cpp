#include "UIUser.h"
#include <gear/input/abstract_input/elements/AKeyboardButton.h>
#include <gear/input/abstract_input/elements/AControllerAxisButton.h>
#include <gear/input/abstract_input/elements/AControllerButton.h>
#include "UI.h"

gear::KUIInput::KUIInput(void)
{
    up = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::W), GEAR_CREATE_BUTTON(AKeyboardButton, Key::UP));
    right = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::D), GEAR_CREATE_BUTTON(AKeyboardButton, Key::RIGHT));
    down = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::S), GEAR_CREATE_BUTTON(AKeyboardButton, Key::DOWN));
    left = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::A), GEAR_CREATE_BUTTON(AKeyboardButton, Key::LEFT));
    submit = GEAR_CREATE_BUTTON(AKeyboardButton, Key::ENTER);
}

gear::CUIInput::CUIInput(int id) : AbstractControllerInput(id)
{
    up = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, -1, -0.4);
    right = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_X, 0.4, 1);
    down = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, 0.4, 1);
    left = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_X, -1, -0.4);
    submit = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::A);
}

gear::UIUser::UIUser(InputDevice device)
{
    m_ID = (int)device;
    m_Input = UIInput::create_From(device);
}

void gear::UIUser::init_Input(void)
{
    // UP
    m_Input->up->set_Callback([this](Action a)
                              {
                                  gear::Ref<gear::UIFocusableComponent> component;
                                  if (a == Action::PRESSED && (component = m_Focused->get_Navigation_Component(UIDirection::UP, m_ID)))
                                  m_Focused = component; });
    // RIGHT
    m_Input->right->set_Callback([this](Action a)
                                 {
                                  gear::Ref<gear::UIFocusableComponent> component;
                                  if (a == Action::PRESSED && (component = m_Focused->get_Navigation_Component(UIDirection::RIGHT, m_ID)))
                                  m_Focused = component; });
    // DOWN
    m_Input->down->set_Callback([this](Action a)
                                {
                                  gear::Ref<gear::UIFocusableComponent> component;
                                  if (a == Action::PRESSED && (component = m_Focused->get_Navigation_Component(UIDirection::DOWN, m_ID)))
                                  m_Focused = component; });
    // LEFT
    m_Input->down->set_Callback([this](Action a)
                                {
                                  gear::Ref<gear::UIFocusableComponent> component;
                                  if (a == Action::PRESSED && (component = m_Focused->get_Navigation_Component(UIDirection::LEFT, m_ID)))
                                  m_Focused = component; });
    // SUBMIT
    m_Input->submit->set_Callback([this](Action a)
                                  {
                                  if (a == Action::PRESSED)
                                     m_Focused->submit(); });
}

int gear::UIUser::getID(void) const
{
    return m_ID;
}

gear::Ref<gear::UIFocusableComponent> gear::UIUser::get_Focused() const{
    return m_Focused;
}

void gear::UIUser::set_Focused(Ref<UIFocusableComponent> component){
    m_Focused = component;
}