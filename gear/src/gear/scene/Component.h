#pragma once

#include <gear/core/core.h>
#include <stdint.h>
#include <gear/data/WeakVector.h>
#include <iostream>

_GEAR_START

class Entity;

unsigned int get_Next_Component_ID(void);

void allow_Gear_Components(void);

template<class T>
/**
A component, that can be added to an entity.
Before a component with a certain type can be used, it has to be allowed.
*/
class Component {
private:
  static unsigned int id;
  static uint64_t flag;
  
  uint8_t m_Scene_ID;
  unsigned int m_Entity_ID;

  Component(void) = default;

  Component(uint8_t scene_ID, unsigned int entity_ID, T data) : m_Scene_ID(scene_ID), m_Entity_ID(entity_ID), data(data) {
    if(id == -1)
      gear::error("Not allowed component used");
  }

  Component(const Component<T> &) = default;

  static void on_Component_Add(Entity entity, T *component){}

  static void on_Component_Remove(Entity entity, T *component){}

public:
  T data;

  /*
  @return the ID of the component. Can be between 0 to GEAR_MAX_COMPONENTS-1 if it is allowed, else it is -1
  */
  static uint64_t get_ID(void) {
    return id;
  }

  /*
  @return the flag of the component. Is 0 if the component is not allowed
  */
  static uint64_t get_Flag(void) {
    return flag;
  }

  /*
  Allows components of this type to be created and added to entites.
  */
  static void allow(void) {
    if(id == -1){
      id = get_Next_Component_ID();
      flag = GEAR_BIT_TYPE(uint64_t, id);
    }
  }

  friend std::ostream& operator<<(std::ostream& ostream, const Component<T>& component)
  {
    ostream << "{scene_ID=" << (int)component.m_Scene_ID << ", entity_ID=" << component.m_Entity_ID << "}";
    return ostream;
  }

  friend class gear::Entity;
  friend class gear::WeakVector<Component<T>>;
};

template<class T>
/*
@return all the flags of the specified types combined
*/
uint64_t component_Flag() {
  return Component<T>::get_Flag();
}

template<class T1, class T2, class ...Ts>
/*
@return all the flags of the specified types combined
*/
uint64_t component_Flag(void) {
  return component_Flag<T2, Ts...>() | Component<T1>::get_Flag();
}

template<class T>
unsigned int Component<T>::id = -1;

template<class T>
uint64_t Component<T>::flag = 0;

_GEAR_END