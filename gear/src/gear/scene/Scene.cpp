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
  entities.create();
  next_ID = 0;
  manager_Callbacks = new ManagerCallbacks[GEAR_MAX_COMPONENTS]{nullptr, nullptr};
  insert_Index = 0;
}

void gear::Scene::destroy(void)
{
  entities.destroy();
  if (manager_Callbacks != nullptr)
  {
    for (int i = 0; i < insert_Index; i++)
      manager_Callbacks[i].destruct_Manager((uint8_t)(this - scenes));
    delete[] manager_Callbacks;
    manager_Callbacks = nullptr;
  }
}

uint8_t gear::Scene::get_ID(void) const
{
  return this - scenes;
}

struct X
{
  int x;
  X(int i) : x(i) {}
};

gear::Entity *gear::Scene::create_Entity(void)
{
  return entities.push_Back({next_ID++, (uint8_t)(this - scenes)});
}

gear::Entity *gear::Scene::create_Entity(void (*constructor)(gear::Entity *entity))
{
  Entity *entity = entities.push_Back({next_ID++, (uint8_t)(this - scenes)});
  constructor(entity);
  return entity;
}

gear::Entity *gear::Scene::find(unsigned int entity_ID)
{
  int count = entities.count();
  if (count == 0)
    return nullptr;
  Entity *data = entities.data();
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
  remove_Entity_At(entity - entities.data());
}

void gear::Scene::remove_Entity_At(int index)
{
  remove_All_Components_On(entities[index].entity_ID);
  entities.remove(index);
}

void gear::Scene::remove_Entity_With_ID(unsigned int entity_ID)
{
  remove_Entity_At(find(entity_ID) - entities.data());
}

gear::Entity *gear::Scene::get_Entity_At(int index)
{
  return entities.data() + index;
}

gear::Entity *gear::Scene::get_Entity_With_ID(unsigned int entity_ID)
{
  return find(entity_ID);
}

void gear::Scene::remove_All_Components_On(unsigned int entity_ID)
{
  for (int i = 0; i < insert_Index; i++)
    manager_Callbacks[i].remove_Entity((uint8_t)(this - scenes), entity_ID);
}

void gear::Scene::add_Manager_Callbacks(gear::ManagerCallbacks callbacks)
{
  GEAR_DEBUG_LOG("Added ManagerCallback");
  manager_Callbacks[insert_Index++] = callbacks;
}

void gear::Scene::print(void)
{
  std::cout << "scene " << this - scenes << ":\n";
  std::cout << "entities:\n";
  if (entities.data() != nullptr)
    for (int i = 0; i < entities.count(); i++)
      std::cout << entities[i].entity_ID << std::endl;
  else
    std::cout << "null\n";
  if (manager_Callbacks != nullptr)
    for (int i = 0; i < insert_Index; i++)
      manager_Callbacks[i].print_Manager(this - scenes);
  else
    std::cout << "no managers\n";
}