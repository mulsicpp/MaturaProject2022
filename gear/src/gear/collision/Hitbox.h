#pragma once

#include <gear/core/core.h>
#include "Hurtbox.h"
#include "CollisionEvent.h"

#include <functional>

_GEAR_START

class Hitbox : public Hurtbox
{
protected:
    std::function<void (CollisionEvent)> m_On_Collision_Begin;
    std::function<void (CollisionEvent)> m_On_Colliding;
    std::function<void (CollisionEvent)> m_On_Collision_End;
public:
    template <class T, class... Ts>
    static Ref<Hitbox> create(uint32_t layer, T shape, Ts... shapes)
    {
        Ref<Hitbox> ret(new Hitbox);
        ret->m_Layer = layer;
        ret->add(shape, shapes...);
        return ret;
    }

    void on_Collision_Begin(std::function<void (CollisionEvent)> action);
    void on_Colliding(std::function<void (CollisionEvent)> action);
    void on_Collision_End(std::function<void (CollisionEvent)> action);
};

_GEAR_END