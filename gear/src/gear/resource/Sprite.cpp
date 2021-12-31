#include "Sprite.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>

gear::Sprite::Sprite(void) : m_TextureID(0), m_Width(0), m_Height(0) {}

gear::Sprite::~Sprite()
{
  GEAR_DEBUG_LOG("delete sprite");
  if(m_TextureID != 0)
    glDeleteTextures(1, &m_TextureID);
}

int gear::Sprite::load(gear::FileStream *file_Stream)
{
  GEAR_DEBUG_LOG("loading sprite ...");
  file_Stream->get<uint16_t>(&m_Width);
  file_Stream->get<uint16_t>(&m_Height);

  uint8_t *data = new uint8_t[m_Width * m_Height];
  file_Stream->get<uint8_t>(data, m_Width * m_Height);

  glGenTextures(1, &m_TextureID);
  glBindTexture(GL_TEXTURE_2D, m_TextureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

  GEAR_DEBUG_LOG("Sprite id: %i", m_TextureID);

  return 0;
}