#pragma once

#include <gear/core/core.h>

_GEAR_START

/*
Copies the values af an array to another array.

@param dest the destination array
@param src the source array
@param count the number of elements to be copied
*/
template<class T>
void memcpy(T *dest, const T *src, int count) {
  while(count--)
    *dest++=*src++;
}

/*
Copies the values af an array to another array.
Here the process takes place backwards. 

@param dest the destination array
@param src the source array
@param count the number of elements to be copied
*/
template<class T>
void memcpy_reverse(T *dest, const T *src, int count) {
  while(count--)
    *dest--=*src--;
}

_GEAR_END