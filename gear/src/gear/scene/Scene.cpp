#include "Entity.h"

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
  m_Comp_Flags.clear();
  m_Next_ID = 0;
  m_Manager_Callbacks = new ManagerCallbacks[GEAR_MAX_COMPONENTS];
  m_Insert_Index = 0;
}

void gear::Scene::destroy(void)
{
  m_Comp_Flags.clear();
  if (m_Manager_Callbacks != nullptr)
  {
    for (int i = 0; i < m_Insert_Index; i++)
    {
      m_Manager_Callbacks[i].destruct_Manager((uint8_t)(this - scenes));
    }
    delete[] m_Manager_Callbacks;
    m_Manager_Callbacks = nullptr;
  }
}

uint8_t gear::Scene::get_ID(void) const
{
  return (uint8_t)(this - scenes);
}

struct X
{
  int x;
  X(int i) : x(i) {}
};

gear::Entity gear::Scene::create_Entity(void)
{
  m_Comp_Flags[m_Next_ID] = 0;
  return {m_Next_ID++, (uint8_t)(this - scenes)};
}

void gear::Scene::remove_Entity(Entity entity)
{
  remove_Entity_With_ID(entity.m_Entity_ID);
}

void gear::Scene::remove_Entity_With_ID(unsigned int entity_ID)
{
  remove_All_Components_On(entity_ID);
  m_Comp_Flags.erase(entity_ID);
}

gear::Entity gear::Scene::get_Entity(unsigned int index)
{
  return {index, (uint8_t)(this - scenes)};
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

void gear::Scene::update_Transformation(void) {
  for(auto &[id, flags] : this->m_Comp_Flags) {
    Entity{id, (uint8_t)(this - scenes)}.update_Transformation();
  }
}

void gear::Scene::print(void)
{
  std::cout << "scene " << this - scenes << ":\n";
  std::cout << "entities:\n";
  for (auto &[key, value] : m_Comp_Flags)
    std::cout << key << std::endl;
  if (m_Manager_Callbacks != nullptr)
    for (int i = 0; i < m_Insert_Index; i++)
      m_Manager_Callbacks[i].print_Manager(this - scenes);
  else
    std::cout << "no managers\n";
}