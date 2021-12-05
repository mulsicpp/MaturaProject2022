#pragma once

#include <gear/core/core.h>
#include <gear/core/debug/log.h>
#include "memory.h"

_GEAR_START

template <class T>
class Grid
{
private:
  T *m_Data;
  uint32_t m_Width, m_Height;

public:
  /*
  Creates a grid with no data.
  */
  Grid(void) : m_Data(nullptr), m_Width(0), m_Height(0) {}

  /*
  Creates a grid with a certain width and height.

  @param width the width of the grid
  @param height the height of the grid
  */
  Grid(uint32_t width, uint32_t height) : m_Data(new T[width * height]), m_Width(width), m_Height(height) {}

  /*
  Creates a grid with a certain width and height and fills it with data.

  @param width the width of the grid
  @param height the height of the grid
  @param data 
  */
  Grid(uint32_t width, uint32_t height, T *data) : m_Data(new T[width * height]), m_Width(width), m_Height(height)
  {
    gear::memcpy(m_Data, data, width * height);
  }

  /*
  Copies a grid with its data.
  */
  Grid(const Grid<T> &grid)
  {
    Grid(grid.m_Width, grid.m_Height, grid.m_Data);
  }

  /*
  Destructor
  */
  ~Grid()
  {
    if (m_Data != nullptr)
    {
      delete[] m_Data;
      m_Data = nullptr;
    }
  }

  /*
  @return a pointer to data of the grid
  */
  T *data(void)
  {
    return m_Data;
  }

  /*
  @return the width of the grid
  */
  uint32_t width(void)
  {
    return m_Width;
  }

  /*
  @return the height of the grid
  */
  uint32_t height(void)
  {
    return m_Height;
  }

  /*
  @return the count of elements in the grid. It is the equivalent of width * height
  */
  uint32_t count()
  {
    return m_Width * m_Height;
  }

  /*
  Returns an element at a certain position.

  @param x the x position of the element
  @param x the x position of the element

  @return a reference to the element
  */
  T &operator()(int x, int y)
  {
    return m_Data[x + y * m_Width];
  }

  /*
  Copies a grid with its data.
  */
  Grid &operator=(const Grid &grid)
  {
    this->m_Width = grid.m_Width;
    this->m_Height = grid.m_Height;
    if (m_Data != nullptr)
    {
      delete[] m_Data;
    }
    this->m_Data = new T[m_Width * m_Height];
    gear::memcpy(this->m_Data, grid.m_Data, m_Width * m_Height);
    return *this;
  }
};

_GEAR_END