#pragma once

#include <gear/core/core.h>
#include <vector>
#include <gear/scene/Scene.h>
#include "Hitbox.h"

_GEAR_START

struct CollisionComponent {
  std::vector<Hitbox> hitboxes;
};

void check_Collisions(Scene *scene);

_GEAR_END