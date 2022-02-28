#include "PhysicsComponent.h"

#include <gear/scene/Entity.h>
#include <gear/scene/TransformComponent.h>

#include <gear/core/debug/log.h>

void gear::check_Collisions(gear::Scene *scene)
{
  size_t count;
  int ints = 0;
  Component<PhysicsComponent> *components = scene->get_All_Components<PhysicsComponent>(&count);

  Vector<double, 2> vec;

  for (int i = 0; i < count; i++)
  {
    for (int j = i + 1; j < count; j++)
    {
      for(auto &hitbox1 : components[i].data.hitbox.get_Shapes())
      {
        for(auto &hitbox2 : components[j].data.hitbox.get_Shapes())
        {
          if(hitbox1.absolute_Shape->intersects(hitbox2.absolute_Shape.get(), &vec))
          {
            ints++;
            Entity e1 = Entity{components[i].get_Entity_ID(), scene->get_ID()};
            e1.get<TransformComponent>()->position -= vec;
            //Entity e2 = Entity{components[j].get_Entity_ID(), scene->get_ID()};
            //e2.get<TransformComponent>()->position += vec / 2;

            e1.update_Transformation();
            //e2.update_Transformation();
          }
        }
      }
    }
  }

  GEAR_DEBUG_LOG("collisions: %i", ints);
}