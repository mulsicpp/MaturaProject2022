#pragma once

#include <gear/core/core.h>
#include "Component_Manager.h"
#include <vector>
#include <functional>

_GEAR_START

class Entity {
private:
  unsigned int id;
  static std::vector<std::function<void(void)>> component_Manager_Destructors;
public:
  template<class T>
  bool has_Component() {
    T* array = Component_Manager<T>::get_Instance().getComponents();
  }

  template<class T, class ...Ts>
  static void iterate(void(*function)(T& arg, Ts& ...args)){
    Component_Manager<T> manager = Component_Manager<T>::get_Instance();

    iterate<...Ts>();
  }

  template<class T>
  static void iterate(void(*function)(T& arg)){

  }

  static void add_Manager_Destructor(std::function<void(void)> function);

  static void destruct_Managers(void);
};

_GEAR_END