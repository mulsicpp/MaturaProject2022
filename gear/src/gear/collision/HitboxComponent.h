#pragma once

#include <gear/core/core.h>
#include "Hitbox.h"
#include <vector>

_GEAR_START

struct HitboxComponent {
    std::vector<Ref<Hitbox>> hitboxes;
};

_GEAR_END