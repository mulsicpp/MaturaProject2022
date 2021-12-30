#pragma once

#include <gear/core/core.h>
#include <unordered_map>
#include <string>

#include "Resource.h"

_GEAR_START

class ResourceManager {
private:
  static std::unordered_map<std::string, Ref<Resource>> m_Resources;

public:
  template<class T>
  static Ref<T> get(std::string filename){
    if(m_Resources.find(filename) == m_Resources.end())
    {
      return std::dynamic_pointer_cast<T, Resource>(m_Resources[filename] = Resource::load_Resource(filename.c_str()));
    }
    return std::dynamic_pointer_cast<T, Resource>(m_Resources[filename]);
  }

  static int unload(void);
};

_GEAR_END