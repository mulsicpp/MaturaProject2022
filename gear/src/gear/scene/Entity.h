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
  unsigned int m_Entity_ID;
  uint64_t m_Comp_Flags;
  uint8_t m_Scene_ID;

  Entity(void) = default;
  Entity(unsigned int entity_ID, const uint8_t scene_ID);
  Entity(const Entity&) = default;

  template <class T>
  class ComponentManager
  {
  private:
    WeakVector<Component<T>> m_Components;
    static ComponentManager<T> instances[GEAR_MAX_SCENES];
    int m_Next_Elem;

    void create(void)
    {
      m_Components.create();
      Scene::get(this - instances)->add_Manager_Callbacks({destroy_Instance, remove_Entity, print_Manager});
    }

    void destroy(void) {
      m_Components.destroy();
    }

    static void destroy_Instance(uint8_t scene_ID)
    {
      instances[scene_ID].destroy();
    }

    static void remove_Entity(uint8_t scene_ID, unsigned int entity_ID) {
      Component<T> *comp = instances[scene_ID].find(entity_ID);
      if(comp != nullptr)
        instances[scene_ID].m_Components.remove(comp- instances[scene_ID].m_Components.data());
    }

  public:
    static ComponentManager<T> &get_Instance(uint8_t scene_ID)
    {
      if (instances[scene_ID].m_Components.data() == nullptr)
      {
        instances[scene_ID].create();
      }
      return instances[scene_ID];
    }

    Component<T> *get_Components(size_t *count)
    {
      *count = m_Components.count();
      return m_Components.data();
    }

    Component<T> *find(unsigned int entity_ID)
    {
      int count = m_Components.count();
      if (count == 0)
        return nullptr;
      Component<T> *data = m_Components.data();
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

    void add_Component(Component<T> component)
    {
      Component<T> *data = m_Components.data();
      int count = m_Components.count();
      int i;
      for (i = 0; i < count; i++)
      {
        if (data[i].m_Entity_ID >= component.m_Entity_ID)
          break;
      }
      if(i == count)
        m_Components.push_Back(component);
      if(data[i].m_Entity_ID != component.m_Entity_ID)
        m_Components.insert(component, i);
      else
        m_Components[i].data = component.data;
    }

    void remove_Component(unsigned int entity_ID)
    {
      Component<T> *comp = find(entity_ID);
      if (comp != nullptr)
        m_Components.remove(comp - m_Components.data());
    }

    void reset_Iterator(void) {
      m_Next_Elem = -1;
    }

    T &iterate_To(unsigned int entity_ID)
    {
      while(m_Components[++m_Next_Elem].m_Entity_ID < entity_ID);
      return m_Components[m_Next_Elem].data;
    }

    void print(void) {
      std::cout << "component manager " << Component<T>::get_ID() << ":\n";
      Component<T> *data = m_Components.data();
      int count = m_Components.count();
      for(int i = 0; i < count; i++)
        std::cout << data[i] << std::endl;
    }

  private:
    static void print_Manager(uint8_t scene_ID) {
      instances[scene_ID].print();
    }
  };

public:
  template <class T1, class T2, class ... Ts>
  /*
  Checks if this entity has a component of the specified type.
  @param T the type of the component
  @return true, if the component is present, else false
  */
  bool has()
  {
    return component_Flag<T1, T2, Ts...>() == (m_Comp_Flags & component_Flag<T1, T2, Ts...>()) ? true : false;
  }

  template <class T>
  /*
  Checks if this entity has a component of the specified type.
  @param T the type of the component
  @return true, if the component is present, else false
  */
  bool has()
  {
    return component_Flag<T>()  & m_Comp_Flags ? true : false;
  }

  template <class T>
  /*
  Adds a component of type T to the entity. If the component already exists
  it will be overwritten.
  @param T the type of the component
  @param component the data of the component
  */
  void add(T component)
  {
    ComponentManager<T>::get_Instance(m_Scene_ID).add_Component(Component<T>{m_Scene_ID, m_Entity_ID, component});
    m_Comp_Flags |= Component<T>::get_Flag();
  }

  template <class T>
  /*
  Removes the component of type T from the entity if it exists.
  @param T the type of the component
  */
  void remove(void)
  {
    ComponentManager<T>::get_Instance(m_Scene_ID).remove_Component(m_Entity_ID);
    m_Comp_Flags &= ~Component<T>::get_Flag();
  }

  template <class T>
  /*
  Gets the component of type T from the entity.
  @param T the type of the component
  @return a pointer to the component if it exists, else a null pointer
  */
  Component<T> *get(void)
  {
    return ComponentManager<T>::get_Instance(m_Scene_ID).find(m_Entity_ID);
  }

  template<class T>
  /*
  Sets the component of type T in the entity. If the component does not exist
  nothing will be set.
  @param T the type of the component
  @param component the data of the component
  */
  void set(T component) {
    if(Component<T>::get_Flag() & m_Comp_Flags)
      ComponentManager<T>::get_Instance(m_Scene_ID).find(m_Entity_ID)->data = component;
  }

  /*
  Removes all components from the entity.
  */
  void remove_All(void);

  /*
  @return the id of the entity
  */
  unsigned int get_Entity_ID(void) const;

  /*
  @return the id of the scene
  */
  uint8_t get_Scene_ID(void) const;

  /*
  @return the flags, containing the added components
  */
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


public:
  template <class... T>
  /*
  Iterates over all entities, that contain the components of the specified types.

  @param T the types of the components
  @param function the function, that gets called with the components
  */
  static void for_Each(uint8_t scene_ID, void (*function)(T &...args))
  {
    WeakVector<Entity> entities = Scene::get(scene_ID)->m_Entities;
    reset_Iterators(ComponentManager<T>::get_Instance(scene_ID)...);
    for (int i = 0; i < entities.count(); i++)
    {
      if (entities[i].has<T...>())
        function(ComponentManager<T>::get_Instance(scene_ID).iterate_To(entities[i].m_Entity_ID)...);
    }
  }

  friend class gear::Scene;
  friend class gear::WeakVector<Entity>;
};

template <class T>
Entity::ComponentManager<T> Entity::ComponentManager<T>::instances[GEAR_MAX_SCENES];

_GEAR_END