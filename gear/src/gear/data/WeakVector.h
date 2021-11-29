#pragma once

#include <gear/core/core.h>
#include "memory.h"
#include <string>

_GEAR_START

/*
This vector isn't safe to use and is only used by the
engine itself, where performance is very important.
*/
template <class T, int B>
class WeakVector
{
private:
  T *_data;
  unsigned int _count;
  unsigned int physical_Count;

  static const unsigned int block_Size;

public:
  void create(void)
  {
    _data = new T[block_Size];
    _count = 0;
    physical_Count = block_Size;
  }

  void destroy(void)
  {
    if (_data != nullptr)
    {
      delete[] _data;
      _data = nullptr;
    }
  }

  WeakVector(void)
  {
    _data = nullptr;
    _count = 0;
    physical_Count = 0;
  }

  T &operator[](int index)
  {
    return data[index];
  }

  unsigned int count(void)
  {
    return _count;
  }

  T *data(void)
  {
    return _data;
  }

  T *push_Back(T element)
  {
    if (_count == physical_Count)
    {
      physical_Count += block_Size;
      T *temp = new T[physical_Count];
      gear::memcpy(temp, _data, _count);
      delete[] _data;
      _data = temp;
    }
    return &(_data[_count++] = element);
  }

  void pop_Back()
  {
    _count--;
    if (_count != 0 && physical_Count - _count == block_Size)
    {
      physical_Count -= block_Size;
      T *temp = new T[physical_Count];
      gear::memcpy(temp, _data, _count);
      delete[] _data;
      _data = temp;
    }
  }

  T *insert(T element, int index)
  {
    if (_count == physical_Count)
    {
      physical_Count += block_Size;
      T *temp = new T[physical_Count];
      gear::memcpy(temp, _data, index);
      gear::memcpy_reverse(temp + _count, _data + _count - 1, _count - index);
      delete[] _data;
      _data = temp;
      _count++;
      return &(_data[index] = element);
    }
    else
    {
      gear::memcpy_reverse(_data + _count, _data + _count - 1, _count - index);
      _count++;
      return &(_data[index] = element);
    }
  }

  void remove(int index)
  {
    _count--;
    if (_count != 0 && physical_Count - _count == block_Size)
    {
      physical_Count -= block_Size;
      T *temp = new T[physical_Count];
      gear::memcpy(temp, _data, index);
      gear::memcpy(temp + index, _data + index + 1, _count - index);
      delete[] _data;
      _data = temp;
    }
    else
    {
      gear::memcpy(_data + index, _data + index + 1, _count - index);
    }
  }

  void print(const char *format)
  {
    printf("{ count=%i, physical count=%i, data={", _count, physical_Count);
    if (_data != nullptr)
      for (int i = 0; i < physical_Count; i++)
        printf(format, _data[i]);
    else
      printf("null");
    printf("}}\n");
  }
};

template <class T, int B>
const unsigned int WeakVector<T, B>::block_Size = B;

_GEAR_END