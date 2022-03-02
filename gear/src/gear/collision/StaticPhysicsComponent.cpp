#include "StaticPhysicsComponent.h"

#include <gear/scene/Entity.h>
#include <gear/scene/TransformComponent.h>

#include <gear/core/debug/log.h>

bool gear::default_Physics_Check(gear::Vector<double, 2> push_Direction, bool pre_Intersect, Entity entity1, Entity entity2)
{
  return true;
}