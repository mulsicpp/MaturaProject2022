#include "UIComponent.h"
#include <gear/scene/TransformComponent.h>

gear::UIComponent::UIComponent(const TransformComponent &transform, const AnimationComponent &default_Animation)
    : m_Transform(transform), m_Default_Animation(default_Animation)
{
    m_Displayed_Animation = m_Default_Animation;
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

gear::AnimationComponent gear::UIComponent::get_Default_Animation(void) const
{
    return m_Default_Animation;
}

void gear::UIComponent::set_Default_Animation(const AnimationComponent &default_Animation)
{
    m_Default_Animation = default_Animation;
    m_Displayed_Animation = m_Default_Animation;
}
