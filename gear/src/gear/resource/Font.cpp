#include "Font.h"

#include <gear/data/FileStream.h>
#include <glad/glad.h>
#include <string.h>

gear::Font::Font(void) : m_Atlas(), m_Flags(0) {}

gear::Font::Font(const char *filename)
{
  load(filename);
}

int gear::Font::load(const char *filename)
{
  GEAR_DEBUG_LOG("loading font ...");
  FileStream *stream = FileStream::open(filename, "rb");
  if(stream == nullptr)
    return -1;
  
  char signature[7]{0};
  stream->get<char>(signature, 7);
  if(memcmp("GEARFNT", signature, 7))
    return -2;

  stream->get<uint8_t>(&m_Flags);
  uint8_t color_Count = 0;
  stream->get<uint8_t>(&color_Count);
  if(color_Count == 0 || color_Count > 255)
    return -2;

  uint8_t bits_Per_Pixel = 0;
  stream->get<uint8_t>(&bits_Per_Pixel);

  stream->get<int16_t>(&m_Char_Gap);
  stream->get<int16_t>(&m_Line_Gap);
  

  stream->get<uint16_t>(&m_Width);
  stream->get<uint16_t>(&m_Height);

  uint32_t data_Size;
  stream->get<uint32_t>(&data_Size);

  uint8_t char_Count = 0;
  stream->get<uint8_t>(&char_Count);

  uint16_t offset = 0;
  char character;
  uint16_t char_Width;
  for(int i = 0; i < char_Count; i++)
  {
    stream->get<char>(&character);
    stream->get<uint16_t>(&char_Width);
    m_Characters[character] = {offset, (uint16_t)(offset + char_Width), 0, m_Height};
  }

  uint8_t bit_Mask = 0;
  for(int i = 0; i < bits_Per_Pixel; i++)
    bit_Mask |= (1 << i);
  uint8_t pixels_Per_Byte = 8 / bits_Per_Pixel;

  uint32_t atlas_Count = m_Width * m_Height;
  uint8_t* atlas_Data = new uint8_t[atlas_Count];
  uint8_t current_Byte;
  for(int i = 0; i < atlas_Count; i++)
  {
    if(i % pixels_Per_Byte == 0)
      stream->get<uint8_t>(&current_Byte);
    atlas_Data[i] = (current_Byte >> ((pixels_Per_Byte - 1 - (i % pixels_Per_Byte)) * bits_Per_Pixel)) & bit_Mask;
  }

  glGenTextures(1, &m_Atlas);
  glBindTexture(GL_TEXTURE_2D, m_Atlas);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_Data);

  GEAR_DEBUG_LOG("Font atlas id: %i", m_Atlas);

  FileStream::close(stream);
}