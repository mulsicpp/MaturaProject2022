#include "HitboxEvent.h"

gear::HitboxEvent::HitboxEvent(Entity entity, Entity other_Entity, uint32_t layer)
    : m_Entity(entity), m_Other_Entity(other_Entity), m_Layer(layer)
{
}

gear::Entity gear::HitboxEvent::get_Entity(void) const { return m_Entity; }
gear::Entity gear::HitboxEvent::get_Other_Entity(void) const { return m_Other_Entity; }
uint32_t gear::HitboxEvent::get_Layer(void) const { return m_Layer; }