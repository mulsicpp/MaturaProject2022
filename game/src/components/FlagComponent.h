#pragma once

#include <stdint.h>

#define FLAG_FIGHTER 0x01
#define FLAG_PLATFORM 0x02
#define FLAG_PROJECTILE 0x04

struct FlagComponent {
    uint64_t flags;
};