#pragma once

#include <gear/core/core.h>
#include "UIFocusableComponent.h"
#include <gear/input/abstract_input/abstract_input.h>
#include <gear/input/abstract_input/AbstractButton.h>

_GEAR_START

GEAR_ABSTRACT_INPUT(UIInput, KUIInput, CUIInput, GEAR_BUTTONS(up, right, down, left, submit, back));

class UI;
class UIUser
{

friend class UI;

private:
    int m_ID;
    Ref<UIFocusableComponent> m_Focused;
    gear::Ref<UIInput> m_Input;
    UIUser(InputDevice device);
    void init_Input(void);
public: 
    int getID(void) const;
    Ref<UIFocusableComponent> get_Focused(void) const;
    void set_Focused(Ref<UIFocusableComponent> component);
};

_GEAR_END