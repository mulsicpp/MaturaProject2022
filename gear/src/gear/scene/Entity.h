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
    WeakVector<Component<T>> components;
    static ComponentManager<T> instances[GEAR_MAX_SCENES];
    int next_Elem;

    void create(void)
    {
      components.create();
      Scene::get(this - instances)->add_Manager_Callbacks({destroy_Instance, remove_Entity, print_Manager});
    }

    void destroy(void) {
      components.destroy();
    }

    static void destroy_Instance(uint8_t scene_ID)
    {
      instances[scene_ID].destroy();
    }

    static void remove_Entity(uint8_t scene_ID, unsigned int entity_ID) {
      Component<T> *comp = instances[scene_ID].find(entity_ID);
      if(comp != nullptr)
        instances[scene_ID].components.remove(comp- instances[scene_ID].components.data());
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
        if (data[i].entity_ID >= component.entity_ID)
          break;
      }
      if(i == count)
        components.push_Back(component);
      if(data[i].entity_ID != component.entity_ID)
        components.insert(component, i);
      else
        components[i].data = component.data;
    }

    void remove_Component(unsigned int entity_ID)
    {
      Component<T> *comp = find(entity_ID);
      if (comp != nullptr)
        components.remove(comp - components.data());
    }

    void reset_Iterator(void) {
      next_Elem = -1;
    }

    T &iterate_To(unsigned int entity_ID)
    {
      while(components[++next_Elem].entity_ID < entity_ID);
      return components[next_Elem].data;
    }

    void print(void) {
      std::cout << "component manager " << Component<T>::get_ID() << ":\n";
      Component<T> *data = components.data();
      int count = components.count();
      for(int i = 0; i < count; i++)
        std::cout << data[i] << std::endl;
    }

  private:
    static void print_Manager(uint8_t scene_ID) {
      instances[scene_ID].print();
    }
  };

public:
  /*
  Checks if this entity has a component of the specified type.
  @param T the type of the component
  @return true, if the component is present, else false
  */
  template <class T1, class T2, class ... Ts>
  bool has()
  {
    return component_Flag<T1, T2, Ts...>() == (comp_Flags & component_Flag<T1, T2, Ts...>()) ? true : false;
  }

  template <class T>
  bool has()
  {
    return component_Flag<T>()  & comp_Flags ? true : false;
  }

  template <class T>
  void add(T component)
  {
    ComponentManager<T>::get_Instance(scene_ID).add_Component(Component<T>{scene_ID, entity_ID, component});
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
    if(Component<T>::get_Flag() & comp_Flags)
      ComponentManager<T>::get_Instance(scene_ID).find(entity_ID)->data = component;
  }

  void remove_All(void);

  unsigned int get_Entity_ID(void) const;
  uint8_t get_Scene_ID(void) const;
  uint64_t get_Component_Flags(void) const;

private:
  template<class T1, class T2, class ...Ts>
  static void reset_Iterators(ComponentManager<T1> &iterator1, ComponentManager<T2> &iterator2, ComponentManager<Ts>& ...iterators)
  {
    iterator1.reset_Iterator();
    reset_Iterators(iterator2, iterators...);
  }

  template<class T>
  static void reset_Iterators(ComponentManager<T> &iterator)
  {
    iterator.reset_Iterator();
  }

  friend class gear::Scene;
  friend class gear::WeakVector<Entity>;
};

template <class T>
Entity::ComponentManager<T> Entity::ComponentManager<T>::instances[GEAR_MAX_SCENES];

_GEAR_END