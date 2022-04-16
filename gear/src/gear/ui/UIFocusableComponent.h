#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include "UIDirection.h"
#include "UIComponent.h"

_GEAR_START

class UIUser;
class UIContainer;

class UIFocusableComponent : public UIComponent
{

friend class UIContainer;

private:
    Vector<int, 2> m_Grid_Pos;
    Ref<UIFocusableComponent> m_Navigation_Components[4];
    uint32_t m_Access_Flags;
    uint32_t m_Focus_Flags;
    AnimationComponent m_Focus_Animation;

public:
    bool is_Focusable(void) const override;

    void set_Grid_Pos(Vector<int, 2> position);
    Vector<int, 2> get_Grid_Pos(void) const;

    void set_Navigation_Component(UIDirection direction, Ref<UIFocusableComponent> component);
    Ref<UIFocusableComponent> get_Navigation_Component(UIDirection direction, int user_ID) const;

    void allow_Access(int user_ID);
    void deny_Access(int user_ID);
    bool has_Access(int user_ID) const;

    void focus_From(int user_ID);
    void unfocus_From(int user_ID);
    bool is_Focused_By(int user_ID) const;

    void gear::UIFocusableComponent::set_Animation(Ref<Animation> animation, Ref<Palette> palette);

    virtual void submit(void) = 0;
};

_GEAR_END