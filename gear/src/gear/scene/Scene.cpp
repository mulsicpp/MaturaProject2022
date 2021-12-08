#include "Scene.h"
#include "Entity.h"
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
  m_Entities.create();
  m_Next_ID = 0;
  m_Manager_Callbacks = new ManagerCallbacks[GEAR_MAX_COMPONENTS]{nullptr, nullptr};
  m_Insert_Index = 0;
}

void gear::Scene::destroy(void)
{
  m_Entities.destroy();
  if (m_Manager_Callbacks != nullptr)
  {
    for (int i = 0; i < m_Insert_Index; i++)
      m_Manager_Callbacks[i].destruct_Manager((uint8_t)(this - scenes));
    delete[] m_Manager_Callbacks;
    m_Manager_Callbacks = nullptr;
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
  return m_Entities.push_Back({m_Next_ID++, (uint8_t)(this - scenes)});
}

gear::Entity *gear::Scene::create_Entity(void (*constructor)(gear::Entity *entity))
{
  Entity *entity = m_Entities.push_Back({m_Next_ID++, (uint8_t)(this - scenes)});
  constructor(entity);
  return entity;
}

gear::Entity *gear::Scene::find(unsigned int entity_ID)
{
  int count = m_Entities.count();
  if (count == 0)
    return nullptr;
  Entity *data = m_Entities.data();
  int index = 0;
  while (count > 1)
  {
    index = count / 2;
    if (data[index].m_Entity_ID == entity_ID)
    {
      return data + index;
    }
    else if (data[index].m_Entity_ID > entity_ID)
    {
      count = index;
    }
    else
    {
      count -= index + 1;
      data = data + index + 1;
    }
  }
  if (data->m_Entity_ID == entity_ID)
    return data;
  else
    return nullptr;
}

void gear::Scene::remove_Entity(Entity *entity)
{
  remove_Entity_At(entity - m_Entities.data());
}

void gear::Scene::remove_Entity_At(int index)
{
  remove_All_Components_On(m_Entities[index].m_Entity_ID);
  m_Entities.remove(index);
}

void gear::Scene::remove_Entity_With_ID(unsigned int entity_ID)
{
  remove_Entity_At(find(entity_ID) - m_Entities.data());
}

gear::Entity *gear::Scene::get_Entity_At(int index)
{
  return m_Entities.data() + index;
}

gear::Entity *gear::Scene::get_Entity_With_ID(unsigned int entity_ID)
{
  return find(entity_ID);
}

void gear::Scene::remove_All_Components_On(unsigned int entity_ID)
{
  for (int i = 0; i < m_Insert_Index; i++)
    m_Manager_Callbacks[i].remove_Entity((uint8_t)(this - scenes), entity_ID);
}

void gear::Scene::add_Manager_Callbacks(gear::ManagerCallbacks callbacks)
{
  m_Manager_Callbacks[m_Insert_Index++] = callbacks;
}

void gear::Scene::print(void)
{
  std::cout << "scene " << this - scenes << ":\n";
  std::cout << "entities:\n";
  if (m_Entities.data() != nullptr)
    for (int i = 0; i < m_Entities.count(); i++)
      std::cout << m_Entities[i].m_Entity_ID << std::endl;
  else
    std::cout << "null\n";
  if (m_Manager_Callbacks != nullptr)
    for (int i = 0; i < m_Insert_Index; i++)
      m_Manager_Callbacks[i].print_Manager(this - scenes);
  else
    std::cout << "no managers\n";
}