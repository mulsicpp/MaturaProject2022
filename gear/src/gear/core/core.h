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
#else
#error Gear only supports windows at the moment
#endif

#define GEAR_TYPE_PUN(var, type) (*(type *)(&var))
#define GEAR_TYPE_PUN_POINTER(var, type) ((type *)(&var))

#include <stdio.h>
#include <stdlib.h>

_GEAR_START

template <class... T>
void error(const char *str, T... args)
{
  printf(str, args...);
  exit(1);
}

_GEAR_END