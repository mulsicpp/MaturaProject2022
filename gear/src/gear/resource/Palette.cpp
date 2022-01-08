#include "Palette.h"
#include <gear/core/debug/log.h>

gear::Palette::Palette(void) : m_Size(0), m_Colors(nullptr) {}

gear::Palette::~Palette() {
  GEAR_DEBUG_LOG("delete palette");
  if(m_Colors != nullptr)
    delete[] m_Colors;
}

int gear::Palette::load(gear::FileStream *file_Stream)
{
  GEAR_DEBUG_LOG("loading palette ...");
  if(m_Colors != nullptr)
    delete[] m_Colors;
  file_Stream->get<uint8_t>(&m_Size);
  m_Colors = new uint32_t[m_Size];
  file_Stream->get<uint32_t>(m_Colors, m_Size);

  GEAR_DEBUG_LOG("palette addr: %p", m_Colors);

  return 0;
}

uint8_t gear::Palette::get_Size(void) const { return m_Size; }
uint32_t *gear::Palette::get_Colors(void) const { return m_Colors; }