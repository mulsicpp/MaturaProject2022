#pragma once

#include <gear/core/core.h>
#include <functional>

_GEAR_START

class Entity;

struct ManagerCallbacks
{
  void (*destruct_Manager)(uint8_t);
  void (*remove_Entity)(uint8_t, unsigned int);
};

class Scene
{
private:
  Entity *entities;
  unsigned int entity_Count, physical_Entity_Count;
  unsigned int next_ID;

  static Scene scenes[GEAR_MAX_SCENES];
  static unsigned int block_Size;

  ManagerCallbacks *manager_Callbacks;
  uint8_t insert_Index;
  Scene(void);

  Entity *find(unsigned int entity_ID);

public:
  Scene(const Scene &scene) = delete;

  static Scene *const get(uint8_t scene_ID);

  void create(void);
  void destroy(void);

  uint8_t get_ID(void) const;

  Entity *create_Entity(void);
  //Entity *create_Entity(void(*constructor)(Entity *entity));

  void remove_Entity(Entity *entity);
  void remove_Entity_At(int index);
  void remove_Entity_With_ID(unsigned int entity_ID);

  Entity *get_Entity_At(int index);
  Entity *get_Entity_With_ID(unsigned int entity_ID);

  void remove_All_Components_On(unsigned int entity_ID);

  void add_Manager_Callbacks(ManagerCallbacks callbacks);
};

_GEAR_END