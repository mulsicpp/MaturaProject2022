#pragma once

#include <gear/core/core.h>
#include <functional>
#include <gear/data/WeakVector.h>
#include <stdint.h>
#include "Component.h"

_GEAR_START

class Entity;

struct ManagerCallbacks
{
  void (*destruct_Manager)(uint8_t);
  void (*remove_Entity)(uint8_t, unsigned int);
  void (*print_Manager)(uint8_t);
};

class Scene
{
private:
  WeakVector<Entity> entities;
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
  Entity *create_Entity(void(*constructor)(Entity *entity));

  void remove_Entity(Entity *entity);
  void remove_Entity_At(int index);
  void remove_Entity_With_ID(unsigned int entity_ID);

  Entity *get_Entity_At(int index);
  Entity *get_Entity_With_ID(unsigned int entity_ID);

  void remove_All_Components_On(unsigned int entity_ID);

  void add_Manager_Callbacks(ManagerCallbacks callbacks);

  template<class... T>
  void for_Each(void(*function)(T&...args)){
    uint8_t scene_ID = this - scenes;
    Entity::reset_Iterators(Entity::ComponentManager<T>::get_Instance(scene_ID)...);
    for(int i = 0; i < entities.count(); i++) {
      if(entities[i].has<T...>())
        function(Entity::ComponentManager<T>::get_Instance(scene_ID).iterate_To(entities[i].entity_ID)...);
    }
  }

  void print(void);
};

_GEAR_END