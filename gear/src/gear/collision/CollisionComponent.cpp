#include "CollisionComponent.h"

#include <gear/scene/Entity.h>
#include <gear/scene/TransformComponent.h>

#include <gear/core/debug/log.h>

void gear::check_Collisions(gear::Scene *scene)
{
  size_t count;
  int ints = 0;
  Component<CollisionComponent> *components = scene->get_All_Components<CollisionComponent>(&count);

  Vector<double, 2> vec;

  for (int i = 0; i < count; i++)
  {
    for (int j = i + 1; j < count; j++)
    {
      for(auto &hitbox1 : components[i].data.hitboxes)
      {
        for(auto &hitbox2 : components[j].data.hitboxes)
        {
          if(hitbox1.m_Absolute_Shape->intersects(hitbox2.m_Absolute_Shape.get(), &vec))
          {
            ints++;
            Entity e = Entity{components[i].get_Entity_ID(), scene->get_ID()};
            auto transform = e.get<TransformComponent>();
            transform->position -= vec;
            e.update_Transformation();
          }
        }
      }
    }
  }

  GEAR_DEBUG_LOG("collisions: %i", ints);
}