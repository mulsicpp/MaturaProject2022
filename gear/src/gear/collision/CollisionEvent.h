#pragma once

#include <gear/core/core.h>
#include <gear/scene/Entity.h>
#include <gear/math/Vector.h>

_GEAR_START

class CollisionEvent
{
    friend void physics_Step(gear::Scene *scene);
protected:
    Entity m_Entity, m_Other_Entity;
    Vector<double, 2> m_Separation_Vector;
    bool m_Previous_Intersection;

    CollisionEvent(Entity entity, Entity other_Entity, Vector<double, 2> separation_Vector, bool previous_Intersection);

public:
    Entity get_Entity(void) const;
    Entity get_Other_Entity(void) const;
    Vector<double, 2> get_Separation_Vector(void) const;
    bool did_Intersect(void) const;
};

_GEAR_END