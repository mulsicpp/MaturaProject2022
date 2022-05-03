#pragma once

#include <gear/core/core.h>
#include <unordered_map>
#include <string>

#include "Resource.h"
#include <filesystem>

_GEAR_START

class ResourceManager
{
private:
    static std::unordered_map<std::string, Ref<Resource>> m_Resources;

public:
    template <class T>
    static Ref<T> get(std::string filename)
    {
        std::string key = std::filesystem::proximate(filename, std::filesystem::current_path()).string();
        if (m_Resources.find(key) == m_Resources.end())
        {
            auto resource = Resource::load_Resource(key.c_str());
            if (resource != nullptr)
                return std::dynamic_pointer_cast<T, Resource>(m_Resources[key] = resource);
            else {
                GEAR_DEBUG_LOG("resource \'%s\' couldn't be loaded", key.c_str());
                return nullptr;
            }
        }
        return std::dynamic_pointer_cast<T, Resource>(m_Resources[key]);
    }

    static int unload(void);
};

_GEAR_END