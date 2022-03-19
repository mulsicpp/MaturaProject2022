#pragma once

#include <gear/core/core.h>
#include <gear/scene/Entity.h>
#include <gear/math/Vector.h>

_GEAR_START

class HitboxEvent
{
    friend void hitbox_Collision_Check(Scene *scene);
protected:
    Entity m_Entity, m_Other_Entity;
    uint32_t m_Layer;

    HitboxEvent(Entity entity, Entity other_Entity, uint32_t layer);

public:
    Entity get_Entity(void) const;
    Entity get_Other_Entity(void) const;
    uint32_t get_Layer(void) const;
};

_GEAR_END