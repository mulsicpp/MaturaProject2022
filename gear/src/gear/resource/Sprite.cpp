#include "Sprite.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>

gear::Sprite::Sprite(void) : m_TextureID(0), m_Width(0), m_Height(0) {}

gear::Sprite::~Sprite()
{
  GEAR_DEBUG_LOG("delete sprite");
  if (m_TextureID != 0)
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

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, data);

  GEAR_DEBUG_LOG("Sprite id: %i", m_TextureID);

  return 0;
}

uint16_t gear::Sprite::get_Width(void) const { return m_Width; }
uint16_t gear::Sprite::get_Height(void) const { return m_Height; }

unsigned int gear::Sprite::get_TextureID(void) const { return m_TextureID; }

gear::Ref<gear::Sprite> gear::Sprite::from_Font(Ref<Font> font) {
    Ref<Sprite> ret(new Sprite());
    ret->m_Width = font->get_Width();
    ret->m_Height = font->get_Height();
    ret->m_TextureID = font->get_TextureID();

    return ret;
}