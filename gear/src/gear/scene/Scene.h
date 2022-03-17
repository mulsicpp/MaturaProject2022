#pragma once

#include <gear/core/core.h>
#include <functional>
#include <map>
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
  friend class gear::Entity;
private:
  std::map<unsigned int, uint64_t> m_Comp_Flags;
  unsigned int m_Next_ID;

  static Scene scenes[GEAR_MAX_SCENES];
  static unsigned int block_Size;

  ManagerCallbacks *m_Manager_Callbacks;
  uint8_t m_Insert_Index;

  Scene(void);
  Scene(const Scene &scene) = delete;
  Scene &operator=(const gear::Scene &) = delete;

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

  @return the new entity
  */
  Entity create_Entity(void);

  /*
  Removes the entity of the specified pointer.

  @param entity the entity to be removed
  */
  void remove_Entity(Entity entity);

  /*
  Removes the entity with the specified id.

  @param entity_ID the id of the entity to be removed
  */
  void remove_Entity_With_ID(unsigned int entity_ID);

  /*
  Returns the entity at the specified index in the array of the scene.

  @param index the index of the entity to be removed
  @return the entity
  */
  Entity get_Entity(unsigned int entity_ID);

  /*
  Removes all components of the entity with the specified id.

  @param entity_ID the id of the entity
  */
  void remove_All_Components_On(unsigned int entity_ID);

  void add_Manager_Callbacks(ManagerCallbacks callbacks);

  /*
  Transforms all entities according to their transform component.
  */
  void update_Transformation(void);

  template<class T>
  Component<T> *get_All_Components(size_t *count) {
    return Entity::ComponentManager<T>::get_Instance(this - scenes).get_Components(count);
  }

  void print(void);
};

_GEAR_END