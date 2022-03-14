#pragma once

#include <gear/core/core.h>
#include "Hitbox.h"
#include <vector>

#include <gear/scene/Scene.h>

_GEAR_START

struct HitboxComponent {
    std::vector<Ref<Hitbox>> hitboxes;
};

void hitbox_Collision_Check(Scene *scene);

_GEAR_END