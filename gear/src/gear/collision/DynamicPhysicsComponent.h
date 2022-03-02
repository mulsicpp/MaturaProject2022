#pragma once

#include <gear/core/core.h>
#include "StaticPhysicsComponent.h"

_GEAR_START

struct DynamicPhysicsComponent : public StaticPhysicsComponent {
  Vector<double, 2> velocity = {0, 0};
  Vector<double, 2> acceleration = {0, 0};
  Vector<double, 2> velocity_X_Interval = {-100, 100};
  Vector<double, 2> velocity_Y_Interval = {-100, 100};
  double mass = 10;
};

void physics_Step(Scene *scene);

_GEAR_END