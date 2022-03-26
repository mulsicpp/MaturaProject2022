#include "Entity.h"
#include "TransformComponent.h"

#include <gear/collision/DynamicPhysicsComponent.h>
#include <gear/collision/HitboxComponent.h>
#include <gear/collision/HurtboxComponent.h>

gear::Entity::Entity(const unsigned int entity_ID, const uint8_t scene_ID) : m_Entity_ID(entity_ID), m_Scene_ID(scene_ID) {}

void gear::Entity::remove_All(void)
{
  Scene::get(m_Scene_ID)->remove_All_Components_On(m_Entity_ID);
}

unsigned int gear::Entity::get_Entity_ID(void) const
{
  return m_Entity_ID;
}

uint8_t gear::Entity::get_Scene_ID(void) const
{
  return m_Scene_ID;
}

void gear::Entity::update_Transformation(void)
{
  if(has<TransformComponent>())
  {
    auto transform = get<TransformComponent>();
    transform->update_Matrix();

    if(has<DynamicPhysicsComponent>())
    {
      auto physics = get<DynamicPhysicsComponent>();
      physics->collider->transform(transform);
    }

    if(has<StaticPhysicsComponent>())
    {
      auto physics = get<StaticPhysicsComponent>();
      physics->collider->transform(transform);
    }

    if(has<HitboxComponent>())
    {
      auto hitbox_Comp = get<HitboxComponent>();
      for(auto &hitbox : hitbox_Comp->hitboxes)
        hitbox->transform(transform);
    }

    if(has<HurtboxComponent>())
    {
      auto hurtbox_Comp = get<HurtboxComponent>();
      for(auto &hurtbox : hurtbox_Comp->hurtboxes)
        hurtbox->transform(transform);
    }
  }
}