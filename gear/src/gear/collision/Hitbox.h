#pragma once

#include <gear/core/core.h>
#include "Hurtbox.h"
#include "HitboxEvent.h"

#include <functional>

_GEAR_START

class Hitbox : public Hurtbox
{
    friend void hitbox_Collision_Check(Scene *scene);
protected:
    std::function<void (HitboxEvent)> m_On_Collision_Begin;
    std::function<void (HitboxEvent)> m_On_Colliding;
    std::function<void (HitboxEvent)> m_On_Collision_End;
public:
    template <class T, class... Ts>
    static Ref<Hitbox> create(uint32_t layer, T shape, Ts... shapes)
    {
        Ref<Hitbox> ret(new Hitbox);
        ret->m_Layer = layer;
        ret->add(shape, shapes...);
        return ret;
    }

    void on_Collision_Begin(std::function<void (HitboxEvent)> action);
    void on_Colliding(std::function<void (HitboxEvent)> action);
    void on_Collision_End(std::function<void (HitboxEvent)> action);
};

_GEAR_END