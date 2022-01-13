#include "Palette.h"
#include <gear/core/debug/log.h>

#include <glad/glad.h>

gear::Palette::Palette(void) : m_Size(0), m_TextureID(0) {}

gear::Palette::~Palette()
{
  GEAR_DEBUG_LOG("delete palette");
  if (m_TextureID != 0)
    glDeleteTextures(1, &m_TextureID);
}

int gear::Palette::load(gear::FileStream *file_Stream)
{
  GEAR_DEBUG_LOG("loading palette ...");
  file_Stream->get<uint8_t>(&m_Size);
  uint32_t data[255]{0};
  file_Stream->get<uint32_t>(data, m_Size);

  glGenTextures(1, &m_TextureID);
  glBindTexture(GL_TEXTURE_1D, m_TextureID);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, 255, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  GEAR_DEBUG_LOG("Font atlas id: %i", m_TextureID);

  return 0;
}

uint8_t gear::Palette::get_Size(void) const { return m_Size; }
unsigned int gear::Palette::get_TextureID(void) const { return m_TextureID; }