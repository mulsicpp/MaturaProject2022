#include "StaticPhysicsComponent.h"

#include <gear/scene/Entity.h>
#include <gear/scene/TransformComponent.h>

#include <gear/core/debug/log.h>

bool gear::default_Physics_Check(CollisionEvent event)
{
  return true;
}