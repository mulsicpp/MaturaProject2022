#pragma once

#include <gear/core/core.h>
#include "ComponentManager.h"
#include <vector>
#include <functional>

_GEAR_START

class Entity {
private:
  unsigned int id;
  uint64_t comp_Flags;
  uint8_t scene_ID;
  static std::vector<std::function<void(void)>> component_Manager_Destructors;
public:
  template<class T>
  bool has_Component() {
    T* array = ComponentManager<T>::get_Instance().getComponents();
    return true;
  }

  template<class T>
  void add_Component(T component) {
    size_t count;
    Component<T>* array = ComponentManager<T>::get_Instance().get_Components(&count);
  }

  static void add_Manager_Destructor(std::function<void(void)> function);

  static void destruct_Managers(void);
};

_GEAR_END