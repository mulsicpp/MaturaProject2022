#pragma once

#include <gear/core/core.h>
#include <vector>
#include <gear/scene/Scene.h>
#include "Hitbox.h"

_GEAR_START

struct PhysicsComponent {
  Hitbox hitbox;
};

void check_Collisions(Scene *scene);

_GEAR_END