#pragma once

#define _GEAR_START \
  namespace gear    \
  {
#define _GEAR_END }

#if defined(_WIN32)
#define GEAR_PLATFORM_WINDOWS
#if defined(GEAR_INCLUDE_WINDOWS_H)
#include <windows.h>
#endif
#elif defined(__linux__)
#define GEAR_PLATFORM_LINUX
#error Gear doesn't support Linux yet
#else
#error Gear is not supported on your platform
#endif

#define GEAR_TYPE_PUN(var, type) (*(type *)(&var))
#define GEAR_TYPE_PUN_POINTER(var, type) ((type *)(&var))

#define GEAR_BIT(x) (1 << (x))
#define GEAR_BIT_TYPE(type, x) ((type)1 << (x))

#if !defined(GEAR_MAX_SCENES)
#define GEAR_MAX_SCENES 8
#else
#if GEAR_MAX_SCENES > 255
#error "Maximum amount of scenes cannot be more than 255"
#endif
#endif

#if !defined(GEAR_MAX_COMPONENTS)
#define GEAR_MAX_COMPONENTS 64
#endif

#include <stdio.h>
#include <stdlib.h>

_GEAR_START

template <class... T>
void error(const char *str, T... args)
{
  printf("\033[0;31m");
  printf(str, args...);
  printf("\033[0m\n");
  exit(1);
}

_GEAR_END