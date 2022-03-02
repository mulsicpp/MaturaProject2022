#pragma once

#include <gear/core/core.h>
#include <vector>
#include <gear/scene/Scene.h>
#include "Hitbox.h"

_GEAR_START

bool default_Physics_Check(Vector<double, 2> push_Direction, bool pre_Intersect, Entity entity1, Entity entity2);

struct PhysicsComponent {
  Hitbox collider;
  Vector<double, 2> velocity = {0, 0};
  Vector<double, 2> acceleration = {0, 0};
  Vector<double, 2> velocity_X_Interval = {-100, 100};
  Vector<double, 2> velocity_Y_Interval = {-100, 100};
  double mass = 10;
  double restitution = 0;
  bool(*check)(Vector<double, 2> push_Direction, bool pre_Intersect, Entity entity1, Entity entity2) = default_Physics_Check;
  bool dynamic = true;
};

void physics_Step(Scene *scene);

_GEAR_END