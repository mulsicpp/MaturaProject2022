#include "UIComponent.h"
#include <gear/scene/TransformComponent.h>

gear::UIComponent::UIComponent(const TransformComponent &transform, Ref<Animation> default_Animation, Ref<Palette> default_Palette, float frame_Offset)
    : m_Transform(transform), m_Default_Animation(default_Animation), m_Default_Palette(default_Palette), m_Frame_Offset(frame_Offset)
{
    m_Displayed_Animation = m_Default_Animation;
    m_Used_Palette = m_Default_Palette;
    m_Transform.update_Matrix();
}

bool gear::UIComponent::is_Focusable(void) const
{
    return false;
}

gear::TransformComponent gear::UIComponent::get_Transform(void) const
{
    return m_Transform;
}

void gear::UIComponent::set_Transform(const TransformComponent &transform)
{
    m_Transform = transform;
    m_Transform.update_Matrix();
}

gear::Ref<gear::Animation> gear::UIComponent::get_Default_Animation(void) const
{
    return m_Default_Animation;
}

void gear::UIComponent::set_Default_Animation(Ref<Animation> default_Animation)
{
    m_Default_Animation = default_Animation;
    m_Displayed_Animation = m_Default_Animation;
}

gear::Ref<gear::Palette> gear::UIComponent::get_Default_Palette(void) const
{
    return m_Default_Palette;
}
void gear::UIComponent::set_Default_Palette(Ref<Palette> default_Palette)
{
    m_Default_Palette = default_Palette;
}
float gear::UIComponent::get_Frame_Offset(void) const
{
    return m_Frame_Offset;
}
void gear::UIComponent::set_Frame_Offset(float frame_Offset)
{
    m_Frame_Offset = frame_Offset;
}
