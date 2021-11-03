#pragma once

#define _GEAR_START namespace gear {
#define _GEAR_END }

#if defined(_WIN32)
  #define GEAR_PLATFORM_WINDOWS
#else
  #error Gear only supports windows at the moment
#endif

#define GEAR_TYPE_PUN(var, type) (*(type*)(&var))
#define GEAR_TYPE_PUN_POINTER(var, type) ((type*)(&var))

#include <stdio.h>
#include <stdlib.h>

#define GEAR_ERROR(...) printf(__VA_ARGS__); exit(1)
