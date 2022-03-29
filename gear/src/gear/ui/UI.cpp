#include "UI.h"

static std::vector<gear::Ref<gear::UIUser>> m_Users;
static gear::Ref<gear::UIContainer> m_Container;

gear::Ref<gear::UIUser> gear::UI::create_User(InputDevice device)
{
    auto user = Ref<UIUser>(new UIUser(device));
    m_Users.push_back(user);
    return user;
}

gear::Ref<gear::UIUser> gear::UI::get_User_By_ID(int id)
{
    for (auto element : m_Users)
        if (element->getID() == id)
            return element;
    return nullptr;
}
gear::Ref<gear::UIUser> gear::UI::get_User_At(int index)
{
    return m_Users.at(index);
}

void gear::UI::remove_User_By_ID(int id)
{
    for (int i = 0; i < m_Users.size(); i++)
        if (m_Users[i]->getID() == id)
            m_Users.erase(m_Users.begin() + i);
}
void gear::UI::remove_User_At(int index)
{
    m_Users.erase(m_Users.begin() + index);
}

void gear::UI::set_Container(Ref<UIContainer> container)
{
    m_Container = container;
}

gear::Ref<gear::UIContainer> gear::UI::get_Container()
{
    return m_Container;
}