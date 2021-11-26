#pragma once

#include <gear/core/core.h>
#include "Component.h"

_GEAR_START

uint64_t get_Next_Component_Flag(void);

template<class T>
class ComponentManager {
private:
  Component<T>* components;
  size_t component_Count;
  static ComponentManager<T> *instance;
  static void destroy_Instance(void);
  ComponentManager<T>(void){

  }

public:
  static ComponentManager<T>& get_Instance(void) {
    if(instance == nullptr){
      instance = new ComponentManager<T>();
    }
    return *instance;
  }

  Component<T>* get_Components(size_t *count) {
    *count = component_Count;
    return components;
  }
};

template<class T>
ComponentManager<T> *ComponentManager<T>::instance = nullptr;

_GEAR_END