#include "PhysicsComponent.h"

#include <gear/scene/Entity.h>
#include <gear/scene/TransformComponent.h>

#include <gear/core/debug/log.h>

void gear::physics_Step(gear::Scene *scene)
{
  size_t count;
  int ints = 0;
  uint8_t scene_ID = scene->get_ID();
  Component<PhysicsComponent> *phy_Comps = scene->get_All_Components<PhysicsComponent>(&count);
  TransformComponent **transform_Comps = new TransformComponent *[count];
  Entity *entities = new Entity[count];

  Entity e1, e2;

  for (int i = 0; i < count; i++)
  {
    entities[i] = Entity{phy_Comps[i].get_Entity_ID(), scene_ID};
    transform_Comps[i] = entities[i].get<TransformComponent>();
    phy_Comps[i].data.velocity += phy_Comps[i].data.acceleration;
    if (phy_Comps[i].data.velocity[0] < phy_Comps[i].data.velocity_X_Interval[0])
      phy_Comps[i].data.velocity[0] = phy_Comps[i].data.velocity_X_Interval[0];

    if (phy_Comps[i].data.velocity[0] > phy_Comps[i].data.velocity_X_Interval[1])
      phy_Comps[i].data.velocity[0] = phy_Comps[i].data.velocity_X_Interval[1];

    if (phy_Comps[i].data.velocity[1] < phy_Comps[i].data.velocity_Y_Interval[0])
      phy_Comps[i].data.velocity[1] = phy_Comps[i].data.velocity_Y_Interval[0];

    if (phy_Comps[i].data.velocity[1] > phy_Comps[i].data.velocity_Y_Interval[1])
      phy_Comps[i].data.velocity[1] = phy_Comps[i].data.velocity_Y_Interval[1];

    transform_Comps[i]->position += phy_Comps[i].data.velocity;

    entities[i].update_Transformation();
  }

  Vector<double, 2> vec;

  for (int i = 0; i < count; i++)
  {
    for (int j = i + 1; j < count; j++)
    {
      for (auto &hitbox1 : phy_Comps[i].data.collider.get_Shapes())
      {
        for (auto &hitbox2 : phy_Comps[j].data.collider.get_Shapes())
        {
          if (hitbox1.absolute_Shape->intersects(hitbox2.absolute_Shape.get(), &vec))
          {
            ints++;
            double inv_Mass1, inv_Mass2;
            if (phy_Comps[i].data.dynamic)
            {
              inv_Mass1 = 1 / phy_Comps[i].data.mass;
              if (phy_Comps[j].data.dynamic)
              {
                inv_Mass2 = 1 / phy_Comps[j].data.mass;
                double sum = phy_Comps[i].data.mass + phy_Comps[j].data.mass;
                transform_Comps[i]->position -= vec * phy_Comps[j].data.mass / sum;
                transform_Comps[j]->position += vec * phy_Comps[i].data.mass / sum;
              }
              else
              {
                inv_Mass2 = 0;
                transform_Comps[i]->position -= vec;
                phy_Comps[j].data.velocity = {0, 0};
              }
            }
            else
            {
              inv_Mass1 = 0;
              phy_Comps[i].data.velocity = {0, 0};
              if (phy_Comps[j].data.dynamic)
              {
                inv_Mass2 = 1 / phy_Comps[j].data.mass;
                transform_Comps[j]->position += vec;
              }
              else
              {
                continue;
              }
            }

            entities[i].update_Transformation();
            entities[j].update_Transformation();

            Vector<double, 2> relative_Speed = phy_Comps[j].data.velocity - phy_Comps[i].data.velocity;
            Vector<double, 2> normal = vec / vec.mag();
            double velocity_Normal = relative_Speed * normal;

            if(velocity_Normal > 0)
              continue;

            double e = phy_Comps[j].data.restitution < phy_Comps[i].data.restitution ? phy_Comps[j].data.restitution : phy_Comps[i].data.restitution;

            double impulse_Scalar = (-(1 + e) * velocity_Normal) / (inv_Mass1 + inv_Mass2);

            Vector<double, 2> impulse = normal * impulse_Scalar;

            phy_Comps[i].data.velocity -= impulse * inv_Mass1;
            phy_Comps[j].data.velocity += impulse * inv_Mass2;
          }
        }
      }
    }
  }

  delete[] transform_Comps;
  delete[] entities;
  GEAR_DEBUG_LOG("collisions: %i", ints);
}