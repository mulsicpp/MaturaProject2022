#include "Hitbox.h"

void gear::Hitbox::on_Collision_Begin(std::function<void(CollisionEvent)> action)
{
    m_On_Collision_Begin = action;
}

void gear::Hitbox::on_Colliding(std::function<void(CollisionEvent)> action)
{
    m_On_Colliding = action;
}

void gear::Hitbox::on_Collision_End(std::function<void(CollisionEvent)> action)
{
    m_On_Collision_End = action;
}