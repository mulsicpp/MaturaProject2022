#pragma once

#include <gear/core/core.h>
#include <vector>
#include <gear/scene/Scene.h>
#include "Hitbox.h"

_GEAR_START

struct PhysicsComponent {
  Hitbox collider;
  Vector<double, 2> velocity = {0, 0};
  Vector<double, 2> acceleration = {0, 0};
  Vector<double, 2> velocity_X_Interval = {-100, 100};
  Vector<double, 2> velocity_Y_Interval = {-100, 100};
  double mass = 10;
  double restitution = 0;
  bool dynamic = true;
};

void physics_Step(Scene *scene);

_GEAR_END