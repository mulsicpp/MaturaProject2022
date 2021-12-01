#pragma once

#include <gear/core/core.h>

_GEAR_START

template<class T>
void memcpy(T *dest, const T *src, int count) {
  while(count--)
    *dest++=*src++;
}

template<class T>
void memcpy_reverse(T *dest, const T *src, int count) {
  while(count--)
    *--dest=*--src;
}

_GEAR_END