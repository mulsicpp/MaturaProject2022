#pragma once

#include <gear/core/core.h>
#include <vector>
#include <functional>
#include "Component.h"
#include "Scene.h"

_GEAR_START

class Scene;

/*
This class represents an entity your game. 
It is part of a scene and can have components attached to it.
*/
class Entity
{
private:
  const unsigned int entity_ID;
  uint64_t comp_Flags;
  uint8_t scene_ID;
  Entity(const unsigned int entity_ID, const uint8_t scene_ID);

  template <class T>
  class ComponentManager
  {
  private:
    std::vector<Component<T>> components;
    static std::vector<ComponentManager<T>> instances;
    static void destroy_Instance(void);

    ComponentManager<T>(uint8_t scene_ID)
    {

      Scene::get_Scene(scene_ID)->add_Manager_Destructor([&](void) {
        //instances.erase(this);
      });
    }

  public:
    static ComponentManager<T> &get_Instance(uint8_t scene_ID)
    {
      if (scene_ID >= instances.size())
      {
        instances.push_back(scene_ID);
      }
      return instances[scene_ID];
    }

    Component<T> *get_Components(size_t *count)
    {
      *count = components.size();
      return components.data();
    }

    Component<T> *find(unsigned int entity_ID)
    {
      int count = components.size();
      if(count == 0)
        return nullptr;
      Component<T> *data = components.data();
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

    void add_Component(Component<T> component)
    {
    }

    void delete_Component(unsigned int entity_ID)
    {
    }
  };

public:
  /*
  Checks if this entity has a component of the specified type.
  @param T the type of the component
  @return true, if the component is present, else false
  */
  template <class T>
  bool
  has_Component()
  {
    return ComponentManager<T>::get_Instance(scene_ID).find(entity_ID) ? true : false;
  }

  template <class T>
  void add_Component(T component)
  {
    ComponentManager<T>::get_Instance(scene_ID).add_Component(Component<T>{entity_ID, component});
  }

  template <class T>
  void delete_Component(void)
  {
    ComponentManager<T>::get_Instance(scene_ID).delete_Component(entity_ID);
  }

  template <class T>
  Component<T> *get_Component(void)
  {
    return ComponentManager<T>::get_Instance(scene_ID).find(entity_ID);
  }

  friend class gear::Scene;
};

template <class T>
std::vector<Entity::ComponentManager<T>> Entity::ComponentManager<T>::instances;

_GEAR_END