#pragma once

#include <gear/core/core.h>
#include <stdint.h>
#include <gear/data/WeakVector.h>
#include <iostream>

_GEAR_START

class Entity;

unsigned int get_Next_Component_ID(void);

template<class T1, class T2, class ...Ts>
uint64_t component_Flag(void) {
  return component_Flag<T2, Ts...>() | Component<T1>::get_Flag();
}

template<class T>
uint64_t component_Flag() {
  return Component<T>::get_Flag();
}

template<class T>
class Component {
private:
  static unsigned int id;
  static uint64_t flag;
  
  uint8_t scene_ID;
  unsigned int entity_ID;

  Component(void) = default;

  Component(uint8_t scene_ID, unsigned int entity_ID, T data) : scene_ID(scene_ID), entity_ID(entity_ID), data(data) {
    if(id == -1)
      gear::error("Not allowed component used");
  }

  Component(const Component<T> &) = default;

public:
  T data;

  static uint64_t get_ID(void) {
    return id;
  }

  static uint64_t get_Flag(void) {
    return flag;
  }

  static void allow(void) {
    if(id == -1){
      id = get_Next_Component_ID();
      flag = GEAR_BIT_TYPE(uint64_t, id);
    }
  }

  friend std::ostream& operator<<(std::ostream& ostream, const Component<T>& component)
  {
    ostream << "{scene_ID=" << (int)component.scene_ID << ", entity_ID=" << component.entity_ID << ", data=" << component.data << "}";
    return ostream;
  }

  friend class gear::Entity;
  friend class gear::WeakVector<Component<T>>;
};

template<class T>
unsigned int Component<T>::id = -1;

template<class T>
uint64_t Component<T>::flag = 0;

_GEAR_END