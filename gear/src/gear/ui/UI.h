#pragma once

#include <gear/core/core.h>
#include <vector>
#include "UIUser.h"
#include "UIContainer.h"

_GEAR_START


class UI
{
private:
    static std::vector<Ref<UIUser>> m_Users;
    static Ref<UIContainer> m_Container;
public:
    static std::vector<Ref<UIUser>> get_Users();
    static Ref<UIUser> create_User(InputDevice device);
    static Ref<UIUser> get_User_By_ID(int id);
    static Ref<UIUser> get_User_At(int index);
    static void remove_User_By_ID(int id);
    static void remove_User_At(int index);

    static void set_Container(Ref<UIContainer> container);
    static Ref<UIContainer> get_Container();
};

_GEAR_END