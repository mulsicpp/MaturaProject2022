#include "UIFocusableComponent.h"
#include "UIUser.h"
#include "UI.h"

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

gear::Ref<gear::UIFocusableComponent> gear::UIFocusableComponent::get_Navigation_Component(UIDirection direction, int user_ID) const
{
    if (!m_Navigation_Components[(int)direction])
        return nullptr;
    if (m_Navigation_Components[(int)direction]->has_Access(user_ID))
        return m_Navigation_Components[(int)direction];
    return m_Navigation_Components[(int)direction]->get_Navigation_Component(direction, user_ID);
}

void gear::UIFocusableComponent::allow_Access(int user_ID)
{
    m_Access_Flags |= (1 << user_ID);
}

void gear::UIFocusableComponent::deny_Access(int user_ID)
{
    m_Access_Flags &= ~(1 << user_ID);
}

bool gear::UIFocusableComponent::has_Access(int user_ID) const
{
    return m_Access_Flags & (1 << user_ID);
}

void gear::UIFocusableComponent::focus_From(int user_ID)
{
    m_Focus_Flags |= (1 << user_ID);
    m_Displayed_Animation = m_Focus_Animation;
    if (m_User_Palette[user_ID] != nullptr)

        m_Used_Palette = m_User_Palette[user_ID];
}

void gear::UIFocusableComponent::unfocus_From(int user_ID)
{
    m_Focus_Flags &= ~(1 << user_ID);
    m_Used_Palette = m_Default_Palette;
    m_Displayed_Animation = m_Default_Animation;
    for (auto user : UI::get_Users())
        if (is_Focused_By(user->getID()))
        {
            m_Displayed_Animation = m_Focus_Animation;
            if (m_User_Palette[user_ID] != nullptr)
                m_Used_Palette = m_User_Palette[user_ID];
            break;
        }
}

bool gear::UIFocusableComponent::is_Focused_By(int user_ID) const
{
    return m_Focus_Flags & (1 << user_ID);
}

void gear::UIFocusableComponent::set_Animation(Ref<Animation> animation)
{
    m_Focus_Animation = animation;
}

void gear::UIFocusableComponent::set_User_Palette(int user_ID, Ref<Palette> palette)
{
    m_User_Palette[user_ID] = palette;
}

gear::Ref<gear::Palette> gear::UIFocusableComponent::get_User_Palette(int user_ID)
{
    return m_User_Palette[user_ID];
}