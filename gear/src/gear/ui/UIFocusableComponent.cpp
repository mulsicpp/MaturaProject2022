#include "UIFocusableComponent.h"
#include "UIUser.h"

bool gear::UIFocusableComponent::is_Focusable(void) const
{
    return true;
}

void gear::UIFocusableComponent::set_Grid_Pos(Vector<int, 2> position)
{
    m_Grid_Pos = position;
}

gear::Vector<int, 2> gear::UIFocusableComponent::get_Grid_Pos(void) const
{
    return m_Grid_Pos;
}

void gear::UIFocusableComponent::set_Navigation_Component(UIDirection direction, Ref<UIFocusableComponent> component)
{
    m_Navigation_Components[(int)direction] = component;
}

gear::Ref<gear::UIFocusableComponent> gear::UIFocusableComponent::get_Navigation_Component(UIDirection direction) const
{
    return m_Navigation_Components[(int)direction];
}

void gear::UIFocusableComponent::allow_Access(UIUser user)
{
    m_Access_Flags |= (1 << user.getID());
}

void gear::UIFocusableComponent::deny_Access(UIUser user)
{
    m_Access_Flags &= ~(1 << user.getID());
}

bool gear::UIFocusableComponent::has_Access(UIUser user) const
{
    return m_Access_Flags & (1 << user.getID());
}

void gear::UIFocusableComponent::focus_From(UIUser user)
{
    m_Focus_Flags |= (1 << user.getID());
}

void gear::UIFocusableComponent::unfocus_From(UIUser user)
{
    m_Focus_Flags &= ~(1 << user.getID());
}

bool gear::UIFocusableComponent::is_Focused_By(UIUser user) const
{
    return m_Focus_Flags & (1 << user.getID());
}
