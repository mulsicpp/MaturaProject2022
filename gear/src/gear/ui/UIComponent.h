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
    friend class UIFocusableComponent;

private:
    TransformComponent m_Transform;
    Ref<Animation> m_Displayed_Animation;
    Ref<Animation> m_Default_Animation;
    Ref<Palette> m_Used_Palette;
    Ref<Palette> m_Default_Palette;
    float m_Frame_Offset;

public:
    UIComponent(void) = default;
    UIComponent(const UIComponent &comp) = default;
    UIComponent(const TransformComponent &transform, Ref<Animation> default_Animation, Ref<Palette> dafault_Palette, float frame_Offset);

    virtual bool is_Focusable(void) const;
    TransformComponent get_Transform(void) const;
    void set_Transform(const TransformComponent &transform);
    Ref<Animation> get_Default_Animation(void) const;
    void set_Default_Animation(Ref<Animation> default_Animation);
    Ref<Palette> get_Default_Palette(void) const;
    void set_Default_Palette(Ref<Palette> default_Palette);
    float get_Frame_Offset(void) const;
    void set_Frame_Offset(float frame_Offset);
};

_GEAR_END