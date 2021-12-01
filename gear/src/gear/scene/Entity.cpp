#include "Entity.h"

gear::Entity::Entity(const unsigned int entity_ID, const uint8_t scene_ID) : entity_ID(entity_ID), comp_Flags(0), scene_ID(scene_ID) {}

void gear::Entity::remove_All(void)
{
  Scene::get(scene_ID)->remove_All_Components_On(entity_ID);
  comp_Flags = 0;
}

unsigned int gear::Entity::get_Entity_ID(void) const
{
  return entity_ID;
}

uint8_t gear::Entity::get_Scene_ID(void) const
{
  return scene_ID;
}

uint64_t gear::Entity::get_Component_Flags(void) const
{
  return comp_Flags;
}