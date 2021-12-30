#include "Palette.h"

gear::Palette::Palette(void) : m_Size(0), m_Colors(nullptr) {}

gear::Palette::~Palette() {
  if(m_Colors != nullptr)
    delete m_Colors;
}

int gear::Palette::load(gear::FileStream *file_Stream)
{
  if(m_Colors != nullptr)
    delete m_Colors;
  file_Stream->get<uint8_t>(&m_Size);
  m_Colors = new uint32_t[m_Size];
  file_Stream->get<uint32_t>(m_Colors, m_Size);
  return 0;
}