#pragma once

#include <gear/core/core.h>
#include "UIFocusableComponent.h"

_GEAR_START

class UIButton : UIFocusableComponent
{
private:
    std::function<void(void)> m_Submit_Callback;
    AnimationComponent m_Pressed_Animation;
    AnimationComponent m_Released_Animation;

public:
    void submit(void);
    void set_Callback(std::function<void(void)> callback);
    std::function<void(void)> get_Callback(void);
};

_GEAR_END