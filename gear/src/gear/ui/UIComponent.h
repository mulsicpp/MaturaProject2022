#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include <gear/scene/Entity.h>
#include <gear/renderer/AnimationComponent.h>
#include <gear/scene/TransformComponent.h>

_GEAR_START

class SpritePipeline;

class UIComponent
{
    friend class SpritePipeline;
private:
    TransformComponent m_Transform;
    AnimationComponent m_Displayed_Animation;
    AnimationComponent m_Default_Animation;

public:
    UIComponent(void) = default;
    UIComponent(const UIComponent &comp) = default;
    UIComponent(const TransformComponent &transform, const AnimationComponent &default_Animation);

    virtual bool is_Focusable(void) const;
    TransformComponent get_Transform(void) const;
    void set_Transform(const TransformComponent &transform);
    AnimationComponent get_Default_Animation(void) const;
    void set_Default_Animation(const AnimationComponent &default_Animation);
};

_GEAR_END