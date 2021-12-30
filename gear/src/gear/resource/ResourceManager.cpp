#include "ResourceManager.h"

std::unordered_map<std::string, gear::Ref<gear::Resource>> gear::ResourceManager::m_Resources;

int gear::ResourceManager::unload(void)
{
  int ret = 0;
  for (auto &item : m_Resources)
  {
    if (item.second.use_count() <= 1)
    {
      m_Resources.erase(item.first);
      ret++;
    }
  }
  return ret;
}