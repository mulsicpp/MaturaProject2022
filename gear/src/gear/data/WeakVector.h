#pragma once

#include <gear/core/core.h>
#include "memcpy.h"
#include <string>

#include <gear/core/debug/log.h>

_GEAR_START

/*
This vector isn't safe to use and is only used by the
engine itself, where performance is very important.
*/
template <class T>
class WeakVector
{
private:
  T *m_Data = nullptr;
  unsigned int m_Count;
  unsigned int m_Capacity;

  static const unsigned int _min_capacity;

public:
  void create(void)
  {
    m_Data = new T[_min_capacity];
    GEAR_DEBUG_LOG("create vector %p", m_Data);
    m_Count = 0;
    m_Capacity = _min_capacity;
  }

  void destroy(void)
  {
    GEAR_DEBUG_LOG("deleting weak vector");
    if (m_Data != nullptr)
    {
      GEAR_DEBUG_LOG("in if %p", m_Data);
      delete[] m_Data;
      m_Data = nullptr;
    }
  }

  WeakVector(void)
  {
    m_Data = nullptr;
    m_Count = 0;
    m_Capacity = 0;
  }

  T &operator[](int index)
  {
    return m_Data[index];
  }

  unsigned int count(void)
  {
    return m_Count;
  }

  unsigned int capacity(void)
  {
    return m_Capacity;
  }

  T *data(void)
  {
    return m_Data;
  }

  T *push_Back(T element)
  {
    if (m_Count == m_Capacity)
    {
      m_Capacity *= 2;
      T *temp = new T[m_Capacity];
      GEAR_DEBUG_LOG("pushback copying %p to %p", m_Data, temp);
      gear::memcpy(temp, m_Data, m_Count);
      delete[] m_Data;
      m_Data = temp;
    }
    return &(m_Data[m_Count++] = element);
  }

  void pop_Back()
  {
    m_Count--;
    if (m_Count > _min_capacity && m_Capacity / m_Count == 2)
    {
      m_Capacity /= 2;
      T *temp = new T[m_Capacity];
      GEAR_DEBUG_LOG("popback copying %p to %p", m_Data, temp);
      gear::memcpy(temp, m_Data, m_Count);
      delete[] m_Data;
      m_Data = temp;
    }
  }

  T *insert(T element, int index)
  {
    if (m_Count == m_Capacity)
    {
      m_Capacity *= 2;
      T *temp = new T[m_Capacity];
      GEAR_DEBUG_LOG("insert copying %p to %p", m_Data, temp);
      gear::memcpy(temp, m_Data, index);
      gear::memcpy_reverse(temp + m_Count + 1, m_Data + m_Count, m_Count - index);
      delete[] m_Data;
      m_Data = temp;
      m_Count++;
      return &(m_Data[index] = element);
    }
    else
    {
      gear::memcpy_reverse(m_Data + m_Count + 1, m_Data + m_Count, m_Count - index);
      m_Count++;
      return &(m_Data[index] = element);
    }
  }

  void remove(int index)
  {
    m_Count--;
    if (m_Count > _min_capacity && m_Capacity / m_Count == 2)
    {
      m_Capacity /= 2;
      T *temp = new T[m_Capacity];
      GEAR_DEBUG_LOG("remove copying %p to %p", m_Data, temp);
      gear::memcpy(temp, m_Data, index * sizeof(T));
      gear::memcpy(temp + index, m_Data + index + 1, m_Count - index);
      delete[] m_Data;
      m_Data = temp;
    }
    else
    {
      gear::memcpy(m_Data + index, m_Data + index + 1, m_Count - index);
    }
  }

  void print(const char *format)
  {
    printf("{ count=%i, capacity=%i, data={", m_Count, m_Capacity);
    if (m_Data != nullptr)
      for (int i = 0; i < m_Capacity; i++)
        printf(format, m_Data[i]);
    else
      printf("null");
    printf("}}\n");
  }
};

template <class T>
const unsigned int WeakVector<T>::_min_capacity = 8;

_GEAR_END