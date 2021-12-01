#pragma once

#include <gear/core/core.h>
#include "memory.h"
#include <string>

_GEAR_START

/*
This vector isn't safe to use and is only used by the
engine itself, where performance is very important.
*/
template <class T>
class WeakVector
{
private:
  T *_data;
  unsigned int _count;
  unsigned int _capacity;

  static const unsigned int _min_capacity;

public:
  void create(void)
  {
    _data = new T[_min_capacity];
    _count = 0;
    _capacity = _min_capacity;
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
    _capacity = 0;
  }

  T &operator[](int index)
  {
    return _data[index];
  }

  unsigned int count(void)
  {
    return _count;
  }

  unsigned int capacity(void)
  {
    return _capacity;
  }

  T *data(void)
  {
    return _data;
  }

  T *push_Back(T element)
  {
    if (_count == _capacity)
    {
      _capacity *= 2;
      T *temp = new T[_capacity];
      gear::memcpy(temp, _data, _count);
      delete[] _data;
      _data = temp;
    }
    return &(_data[_count++] = element);
  }

  void pop_Back()
  {
    _count--;
    if (_count > _min_capacity && _capacity / _count == 2)
    {
      _capacity /= 2;
      T *temp = new T[_capacity];
      gear::memcpy(temp, _data, _count);
      delete[] _data;
      _data = temp;
    }
  }

  T *insert(T element, int index)
  {
    if (_count == _capacity)
    {
      _capacity *= 2;
      T *temp = new T[_capacity];
      gear::memcpy(temp, _data, index);
      gear::memcpy_reverse(temp + _count + 1, _data + _count, _count - index);
      delete[] _data;
      _data = temp;
      _count++;
      return &(_data[index] = element);
    }
    else
    {
      gear::memcpy_reverse(_data + _count + 1, _data + _count, _count - index);
      _count++;
      return &(_data[index] = element);
    }
  }

  void remove(int index)
  {
    _count--;
    if (_count > _min_capacity && _capacity / _count == 2)
    {
      _capacity /= 2;
      T *temp = new T[_capacity];
      gear::memcpy(temp, _data, index * sizeof(T));
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
    printf("{ count=%i, capacity=%i, data={", _count, _capacity);
    if (_data != nullptr)
      for (int i = 0; i < _capacity; i++)
        printf(format, _data[i]);
    else
      printf("null");
    printf("}}\n");
  }
};

template <class T>
const unsigned int WeakVector<T>::_min_capacity = 8;

_GEAR_END