#include "UIButton.h"

void gear::UIButton::set_Callback(std::function<void(void)> callback){
    m_Submit_Callback = callback;
}
std::function<void(void)> gear::UIButton::get_Callback(void){
    return m_Submit_Callback;
}
void gear::UIButton::set_Pressed_Animation(Ref<Animation> animation){
    m_Pressed_Animation = animation;
}
void gear::UIButton::submit(void){
    m_Submit_Callback();
}