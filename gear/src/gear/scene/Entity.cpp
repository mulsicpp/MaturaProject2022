#include "Entity.h"
#include "TransformComponent.h"

#include <gear/collision/PhysicsComponent.h>

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
  auto transform = get<TransformComponent>();
  if(transform)
  {
    transform->update_Matrix();

    if(has<PhysicsComponent>())
    {
      auto physics = get<PhysicsComponent>();
      physics->collider.transform(transform);
    }
  }
}