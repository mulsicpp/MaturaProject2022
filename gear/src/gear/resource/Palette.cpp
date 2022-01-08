#include "Palette.h"
#include <gear/core/debug/log.h>

gear::Palette::Palette(void) : m_Size(0), m_Colors(nullptr) {}

gear::Palette::~Palette()
{
  GEAR_DEBUG_LOG("delete palette");
  if (m_Colors != nullptr)
  {
    delete[] m_Colors;
    m_Colors = nullptr;
  }
}

int gear::Palette::load(gear::FileStream *file_Stream)
{
  GEAR_DEBUG_LOG("loading palette ...");
  if (m_Colors != nullptr)
    delete[] m_Colors;
  file_Stream->get<uint8_t>(&m_Size);
  m_Colors = new Vector<float, 4>[m_Size];
  uint8_t val = 0;
  for (int i = 0; i < m_Size; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      file_Stream->get<uint8_t>(&val);
      m_Colors[i][j] = val / 255.0f;
    }
  }

  GEAR_DEBUG_LOG("palette addr: %p", m_Colors);
  for(int i = 0; i < m_Size; i++)
    GEAR_DEBUG_LOG("color %i: %f %f %f %f", i, m_Colors[i][0], m_Colors[i][1], m_Colors[i][2], m_Colors[i][3]);

  return 0;
}

uint8_t gear::Palette::get_Size(void) const { return m_Size; }
gear::Vector<float, 4> *gear::Palette::get_Colors(void) const { return m_Colors; }