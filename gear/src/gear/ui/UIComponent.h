#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include <gear/scene/Entity.h>
#include <gear/renderer/AnimationComponent.h>

_GEAR_START

class UIComponent
{
private:
    AnimationComponent m_Default_Animation;
    Entity m_Entity;
    UIComponent(Entity entity, Ref<Animation> animation, Ref<Palette> palette, Vector<double, 2> position);
    UIComponent(const UIComponent &container) = delete;
    UIComponent &operator=(const UIComponent &container) = delete;

public:
    virtual bool is_Focusable(void) const;
    Vector<double, 2> get_Position(void) const;
    void set_Position(Vector<double, 2> position);
    void set_Animation(Ref<Animation> animation, Ref<Palette> palette);
};

_GEAR_END