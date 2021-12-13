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

/*
A scene, that can contain an array of entites.
*/
class Scene
{
private:
  WeakVector<Entity> m_Entities;
  unsigned int m_Next_ID;

  static Scene scenes[GEAR_MAX_SCENES];
  static unsigned int block_Size;

  ManagerCallbacks *m_Manager_Callbacks;
  uint8_t m_Insert_Index;

  Scene(void);
  Scene(const Scene &scene) = delete;
  Scene &operator=(const gear::Scene &) = delete;

  Entity *find(unsigned int entity_ID);

public:
  /*
  Gets a pointer to a scene at a certain position.

  @param scene_ID the position of the scene (has to be between 0 and GEAR_MAX_SCENES-1)
  @return a pointer to the desired scene
  */
  static Scene *const get(uint8_t scene_ID);

  /*
  Creates the scene with no entites.
  */
  void create(void);

  /*
  Destroyes the scene.
  */
  void destroy(void);

  /*
  @return the scene id
  */
  uint8_t get_ID(void) const;

  /*
  Creates a new and empty entity.

  @return a pointer to the new entity
  */
  Entity *create_Entity(void);

  /*
  Creates a new entity using a constructor.

  @param constructor a pointer to a function, that adds data to the passed entity.
  @return a pointer to the new entity
  */
  Entity *create_Entity(void (*constructor)(Entity *entity));

  /*
  Removes the entity of the specified pointer.

  @param entity the pointer to the entity to be removed
  */
  void remove_Entity(Entity *entity);

  /*
  Removes the entity at the specified index in the array of the scene.

  @param index the index of the entity to be removed
  */
  void remove_Entity_At(int index);

  /*
  Removes the entity with the specified id.

  @param entity_ID the id of the entity to be removed
  */
  void remove_Entity_With_ID(unsigned int entity_ID);

  /*
  Returns the entity at the specified index in the array of the scene.

  @param index the index of the entity to be removed
  @return a pointer to the entity
  */
  Entity *get_Entity_At(int index);

  /*
  Returns the entity with the specified id.

  @param entity_ID the id of the entity to be removed
  @return a pointer to the entity
  */
  Entity *get_Entity_With_ID(unsigned int entity_ID);

  /*
  Removes all components of the entity with the specified id.

  @param entity_ID the id of the entity
  */
  void remove_All_Components_On(unsigned int entity_ID);

  void add_Manager_Callbacks(ManagerCallbacks callbacks);

  void print(void);
};

_GEAR_END