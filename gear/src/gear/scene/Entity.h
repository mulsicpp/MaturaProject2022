#pragma once

#include <gear/core/core.h>
#include <vector>
#include <functional>
#include "Component.h"
#include "Scene.h"
#include <gear/data/WeakVector.h>

_GEAR_START

/*
This class represents an entity your game. 
It is part of a scene and can have components attached to it.
*/
class Entity
{
private:
  unsigned int entity_ID;
  uint64_t comp_Flags;
  uint8_t scene_ID;

  Entity(void) = default;
  Entity(unsigned int entity_ID, const uint8_t scene_ID);
  Entity(const Entity&) = default;

  template <class T>
  class ComponentManager
  {
  private:
    WeakVector<Component<T>, 8> components;
    static ComponentManager<T> instances[GEAR_MAX_SCENES];

    void create(void)
    {
      components.create();
      Scene::get(this - instances)->add_Manager_Callbacks({destroy_Instance, remove_Entity});
    }

    void destroy(void) {
      components.destroy();
    }

    static void destroy_Instance(uint8_t scene_ID)
    {
      instances[scene_ID].destroy();
    }

    static void remove_Entity(uint8_t scene_ID, unsigned int entity_ID) {

    }

  public:
    static ComponentManager<T> &get_Instance(uint8_t scene_ID)
    {
      if (instances[scene_ID].components.data() == nullptr)
      {
        instances[scene_ID].create();
      }
      return instances[scene_ID];
    }

    Component<T> *get_Components(size_t *count)
    {
      *count = components.count();
      return components.data();
    }

    Component<T> *find(unsigned int entity_ID)
    {
      int count = components.count();
      if (count == 0)
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
      Component<T> *data = components.data();
      int count = components.count();
      int i;
      for (i = 0; i < count; i++)
      {
        if (data[i].entity_ID > component.entity_ID)
          break;
      }
      //components.insert(components.begin() + i, component);
    }

    void remove_Component(unsigned int entity_ID)
    {
      Component<T> *comp = find(entity_ID);
      if (comp != nullptr)
        ;//TODO: implementation
    }
  };

public:
  /*
  Checks if this entity has a component of the specified type.
  @param T the type of the component
  @return true, if the component is present, else false
  */
  template <class... T>
  bool has()
  {
    return component_Flag<T...>() & comp_Flags ? true : false;
  }

  template <class T>
  void add(T component)
  {
    ComponentManager<T>::get_Instance(scene_ID).add_Component(Component<T>{entity_ID, component});
    comp_Flags |= Component<T>::get_Flag();
  }

  template <class T>
  void remove(void)
  {
    ComponentManager<T>::get_Instance(scene_ID).remove_Component(entity_ID);
    comp_Flags &= ~Component<T>::get_Flag();
  }

  template <class T>
  Component<T> *get(void)
  {
    return ComponentManager<T>::get_Instance(scene_ID).find(entity_ID);
  }

  template<class T>
  void set(T component) {
    ComponentManager<T>::get_Instance(scene_ID).find(entity_ID)->data = component;
  }

  void remove_All(void);

  unsigned int get_Entity_ID(void) const;
  uint8_t get_Scene_ID(void) const;
  uint64_t get_Component_Flags(void) const;

  friend class gear::Scene;
};

template <class T>
Entity::ComponentManager<T> Entity::ComponentManager<T>::instances[GEAR_MAX_SCENES];

_GEAR_END