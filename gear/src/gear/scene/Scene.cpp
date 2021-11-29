#include "Scene.h"
#include "Entity.h"
#include <gear/core/debug/log.h>
#include <gear/data/WeakVector.h>

#define GEAR_PHYSICAL_COUNT(count, block_Size) (((count / block_Size) + 1) * block_Size)

gear::Scene gear::Scene::scenes[GEAR_MAX_SCENES];
unsigned int gear::Scene::block_Size = 8;

gear::Scene::Scene(void)
{
  destroy();
}

gear::Scene *const gear::Scene::get(uint8_t scene_ID)
{
  return scenes + scene_ID;
}

void gear::Scene::create(void)
{
  entities = new Entity[block_Size];
  entity_Count = 0;
  physical_Entity_Count = block_Size;
  next_ID = 0;
  manager_Callbacks = new ManagerCallbacks[GEAR_MAX_COMPONENTS]{nullptr, nullptr};
  insert_Index = 0;
}

void gear::Scene::destroy(void) {
  if(entities != nullptr){
    delete[] entities;
    entities = nullptr;
  }
  if(manager_Callbacks != nullptr){
    delete[] manager_Callbacks;
    manager_Callbacks = nullptr;
  }
}

uint8_t gear::Scene::get_ID(void) const
{
  return this - scenes;
}

struct X{
  int x;
  X(int i) : x(i) {}
};

gear::Entity *gear::Scene::create_Entity(void)
{
  if(entity_Count == physical_Entity_Count)
  {
    physical_Entity_Count += block_Size;
    Entity* temp = new Entity[physical_Entity_Count];
    gear::memcpy(temp, entities, entity_Count);
    delete[] entities;
    entities = temp;
  }
  return &(entities[entity_Count++] = {next_ID++, (uint8_t)(this - scenes)});
}

gear::Entity *gear::Scene::find(unsigned int entity_ID)
{
  int count = entity_Count;
  if (count == 0)
    return nullptr;
  Entity *data = entities;
  int index = 0;
  while (count > 1)
  {
    index = count / 2;
    if (data[index].entity_ID == entity_ID)
    {
      return data + index;
    }
    else if (data[index].entity_ID > entity_ID)
    {
      count = index;
    }
    else
    {
      count -= index + 1;
      data = data + index + 1;
    }
  }
  if (data->entity_ID == entity_ID)
    return data;
  else
    return nullptr;
}

void gear::Scene::remove_Entity(Entity *entity)
{
  remove_Entity_At(entity - entities);
}

void gear::Scene::remove_Entity_At(int index)
{
  remove_All_Components_On(entities[index].entity_ID);
}

void gear::Scene::remove_Entity_With_ID(unsigned int entity_ID)
{
  remove_Entity_At(find(entity_ID) - entities);
}

gear::Entity *gear::Scene::get_Entity_At(int index)
{
  return &entities[index];
}

gear::Entity *gear::Scene::get_Entity_With_ID(unsigned int entity_ID)
{
  return find(entity_ID);
}

void gear::Scene::remove_All_Components_On(unsigned int entity_ID) {
  for(int i = 0; i < insert_Index; i++)
    manager_Callbacks[i].remove_Entity((uint8_t)(this - scenes), entity_ID);
}

void gear::Scene::add_Manager_Callbacks(gear::ManagerCallbacks callbacks) {
  manager_Callbacks[insert_Index++] = callbacks;
}