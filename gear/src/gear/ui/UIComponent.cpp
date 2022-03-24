#include "UIComponent.h"
#include <gear/scene/TransformComponent.h>

gear::UIComponent::UIComponent(Entity entity)
{
    m_Entity = entity;
    m_Entity.add<TransformComponent>({{0, 0}, {1, 1}, 0});
}

bool gear::UIComponent::is_Focusable(void) const
{
    return false;
}

void gear::UIComponent::set_Position(Vector<double, 2> position)
{
    m_Entity.get<TransformComponent>()->position = position;
}

gear::Vector<double, 2> gear::UIComponent::get_Position(void) const
{
    return m_Entity.get<TransformComponent>()->position;
}