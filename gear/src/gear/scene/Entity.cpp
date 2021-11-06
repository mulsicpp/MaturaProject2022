#include "Entity.h"

void gear::Entity::add_Manager_Destructor(std::function<void(void)> function)
{
  component_Manager_Destructors.push_back(function);
}

void gear::Entity::destruct_Managers(void) {
  for(std::function<void(void)> function : component_Manager_Destructors)
    function();
  
  component_Manager_Destructors = std::vector<std::function<void(void)>>(0);
}