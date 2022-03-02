#pragma once

#include <gear/core/core.h>
#include <vector>
#include <gear/scene/Scene.h>
#include "Hitbox.h"

_GEAR_START

bool default_Physics_Check(Vector<double, 2> push_Direction, bool pre_Intersect, Entity entity1, Entity entity2);

struct StaticPhysicsComponent {
  Hitbox collider;
  double restitution = 0;
  bool(*check)(Vector<double, 2> push_Direction, bool pre_Intersect, Entity entity1, Entity entity2) = default_Physics_Check;
};

_GEAR_END