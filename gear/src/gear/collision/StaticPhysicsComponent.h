#pragma once

#include <gear/core/core.h>
#include <vector>
#include <gear/scene/Scene.h>
#include "Collider.h"
#include "events/CollisionEvent.h"

_GEAR_START

bool default_Physics_Check(CollisionEvent event);

struct StaticPhysicsComponent {
  Collider collider;
  double restitution = 0;
  bool(*check)(CollisionEvent event) = default_Physics_Check;
  std::function<void(CollisionEvent)> on_Collision;
  std::function<void(CollisionEvent)> on_Collision_Resolved;
};

_GEAR_END