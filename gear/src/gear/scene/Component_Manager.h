#pragma once

#include <gear/core/core.h>

_GEAR_START

template<class T>
class Component_Manager {
private:
  T* components;
  static Component_Manager<T> *instance;
  static void destroy_Instance(void);
  Component_Manager<T>();

public:
  static Component_Manager<T>& get_Instance(void) {
    if(instance == nullptr){
      instance = new Component_Manager<T>();
    }
    return *instance;
  }
};

_GEAR_END