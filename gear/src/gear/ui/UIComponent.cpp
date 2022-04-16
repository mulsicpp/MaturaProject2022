#include "UIComponent.h"
#include <gear/scene/TransformComponent.h>

gear::UIComponent::UIComponent(Entity entity, Ref<Animation> animation, Ref<Palette> palette, Vector<double, 2> position)
{
    m_Entity = entity;
    m_Default_Animation = {animation, palette, AnimationType::LOOP, 0, animation->get_Default_Frame_Rate(), 1, {0, 0, 0}, 1};
    m_Entity.add<TransformComponent>({position, {1, 1}, 0});
    m_Entity.add<AnimationComponent>(m_Default_Animation);
}

bool gear::UIComponent::is_Focusable(void) const
{
    return false;
}

gear::Vector<double, 2> gear::UIComponent::get_Position(void) const
{
    return m_Entity.get<TransformComponent>()->position;
}
void gear::UIComponent::set_Position(Vector<double, 2> position)
{
    m_Entity.get<TransformComponent>()->position = position;
}