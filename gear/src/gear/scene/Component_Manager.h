#pragma once

#include <gear/core/core.h>
#include <stdint.h>

#define GEAR_COMP_FLAG(type) gear::Component_Manager<type>::get_Flag()

_GEAR_START

uint64_t get_Next_Component_Flag(void);

template<class T>
class Component_Manager {
private:
  T* components;
  static inline uint64_t flag = get_Next_Component_Flag();
  static inline Component_Manager<T> *instance;
  static void destroy_Instance(void);
  Component_Manager<T>(void){

  }

public:
  static Component_Manager<T>& get_Instance(void) {
    if(instance == nullptr){
      instance = new Component_Manager<T>();
    }
    gear::Component_Manager<T>::get_Flag();
    return *instance;
  }

  static uint64_t get_Flag(void) {
    return flag;
  }
};

_GEAR_END