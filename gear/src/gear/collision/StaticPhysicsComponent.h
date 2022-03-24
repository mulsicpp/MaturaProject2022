#pragma once

#include <gear/core/core.h>
#include <vector>
#include <gear/scene/Scene.h>
#include "Collider.h"
#include "CollisionEvent.h"

_GEAR_START

bool default_Physics_Check(CollisionEvent event);

struct StaticPhysicsComponent {
  Ref<Collider> collider;
  double restitution = 0;
  std::function<bool(CollisionEvent)> check = default_Physics_Check;
  std::function<void(CollisionEvent)> on_Collision;
  std::function<void(CollisionEvent)> on_Collision_Resolved;

  std::function<void(CollisionEvent)> on_Overlap;
};

_GEAR_END