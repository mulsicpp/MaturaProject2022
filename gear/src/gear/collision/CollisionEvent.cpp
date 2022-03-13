#include "CollisionEvent.h"

gear::CollisionEvent::CollisionEvent(Entity entity, Entity other_Entity, Vector<double, 2> separation_Vector, bool previous_Intersection)
    : m_Entity(entity), m_Other_Entity(other_Entity), m_Separation_Vector(separation_Vector), m_Previous_Intersection(previous_Intersection)
{
}

gear::Entity gear::CollisionEvent::get_Entity(void) const { return m_Entity; }
gear::Entity gear::CollisionEvent::get_Other_Entity(void) const { return m_Other_Entity; }
gear::Vector<double, 2> gear::CollisionEvent::get_Separation_Vector(void) const { return m_Separation_Vector; }
bool gear::CollisionEvent::did_Intersect(void) const { return m_Previous_Intersection; }