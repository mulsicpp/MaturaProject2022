#include "Animation.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>

gear::Animation::Animation(void) : m_TextureID(0), m_Width(0), m_Height(0), m_Frame_Count(0), m_Type(Type::LOOP), m_Frame_Rate(0) {}

gear::Animation::~Animation()
{
  GEAR_DEBUG_LOG("delete animation");
  if (m_TextureID != 0)
    glDeleteTextures(1, &m_TextureID);
}

int gear::Animation::load(gear::FileStream *file_Stream)
{
  GEAR_DEBUG_LOG("load animation ...");

  file_Stream->get<uint16_t>(&m_Width);
  file_Stream->get<uint16_t>(&m_Height);
  file_Stream->get<uint16_t>(&m_Frame_Count);

  uint8_t type;
  file_Stream->get<uint8_t>(&type);
  m_Type = Type(type);
  file_Stream->get<float>(&m_Frame_Rate);

  uint8_t *data = new uint8_t[m_Width * m_Height * m_Frame_Count];
  file_Stream->get<uint8_t>(data, m_Width * m_Height * m_Frame_Count);

  glGenTextures(1, &m_TextureID);
  glBindTexture(GL_TEXTURE_2D, m_TextureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height * m_Frame_Count, 0, GL_RED, GL_UNSIGNED_BYTE, data);

  GEAR_DEBUG_LOG("Animation id: %i", m_TextureID);

  return 0;
}

uint16_t gear::Animation::get_Width(void) const { return m_Width; }
uint16_t gear::Animation::get_Height(void) const { return m_Height; }
uint16_t gear::Animation::get_Frame_Count(void) const { return m_Frame_Count; }

gear::Animation::Type gear::Animation::get_Type(void) const { return m_Type; }

float gear::Animation::get_Frame_Rate(void) const { return m_Frame_Rate; }

unsigned int gear::Animation::get_TextureID(void) const { return m_TextureID; }