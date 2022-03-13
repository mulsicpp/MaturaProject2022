#pragma once

#include <gear/core/core.h>
#include "Collider.h"
#include "CollisionEvent.h"

#include <functional>

_GEAR_START

class Hurtbox : public Collider
{
protected:
    uint32_t m_Layer;
public:
    template <class T, class... Ts>
    static Hurtbox create(uint32_t layer, T shape, Ts... shapes)
    {
        Hurtbox ret;
        ret.m_Layer = layer;
        ret.add(shape, shapes...);
        return ret;
    }

    uint32_t get_Layer(void) const;
    void set_Layer(uint32_t layer);
};

_GEAR_END