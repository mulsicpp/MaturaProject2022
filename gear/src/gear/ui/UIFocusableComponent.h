#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include "UIDirection.h"
#include "UIUser.h"

_GEAR_START

class UIFocusableComponent : public UIComponent
{
private:
    Vector<int, 2> m_Grid_Pos;
    Ref<UIFocusableComponent> m_Navigation_Components[4];
    uint32_t m_Access_Flags;
    uint32_t m_Focus_Flags;

public:
    bool is_Focusable(void) const override;

    void set_Grid_Pos(Vector<int, 2> position);
    Vector<int, 2> get_Grid_Pos(void) const;

    void set_Navigation_Component(UIDirection direction, Ref<UIFocusableComponent> component);
    Ref<UIFocusableComponent> get_Navigation_Component(UIDirection direction) const;

    void allow_Access(UIUser user);
    void deny_Access(UIUser user);
    bool has_Access(UIUser user) const;

    void focus_From(UIUser user);
    void unfocus_From(UIUser user);
    bool is_Focused_By(UIUser user) const;
};

_GEAR_END