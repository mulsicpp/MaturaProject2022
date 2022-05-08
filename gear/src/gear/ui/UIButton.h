#pragma once

#include <gear/core/core.h>
#include "UIFocusableComponent.h"

_GEAR_START

class UIButton : UIFocusableComponent
{
private:
    std::function<void(void)> m_Submit_Callback;
    Ref<Animation> m_Pressed_Animation;

public:
    void set_Callback(std::function<void(void)> callback);
    std::function<void(void)> get_Callback(void);
    void set_Pressed_Animation(Ref<Animation> animation);
    void submit(void);

};

_GEAR_END