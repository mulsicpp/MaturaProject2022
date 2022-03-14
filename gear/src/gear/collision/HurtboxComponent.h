#pragma once

#include <gear/core/core.h>
#include "Hurtbox.h"
#include <vector>

_GEAR_START

struct HurtboxComponent {
    std::vector<Ref<Hurtbox>> hurtboxes;
};

_GEAR_END