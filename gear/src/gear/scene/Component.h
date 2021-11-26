#pragma once

#include <gear/core/core.h>
#include <stdint.h>

_GEAR_START

uint64_t get_Next_Component_Flag(void);

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
  static const uint64_t flag;
public:
  int entity_ID;
  T data;

  static uint64_t get_Flag(void) {
    return flag;
  }
};

template<class T>
const uint64_t Component<T>::flag = get_Next_Component_Flag();

_GEAR_END