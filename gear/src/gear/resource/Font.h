#pragma once

#include <gear/core/core.h>
#include <stdint.h>
#include <unordered_map>
#include <gear/data/Grid.h>
#include "Resource.h"

_GEAR_START

class Font : public Resource
{
private:
  struct CharacterLocation
  {
    uint16_t x_Start;
    uint16_t x_End;
    uint16_t y_Start;
    uint16_t y_End;
  };

  uint8_t m_Flags;
  int16_t m_Char_Gap;
  int16_t m_Line_Gap;
  uint16_t m_Width, m_Height;
  unsigned int m_Atlas;
  std::unordered_map<char, CharacterLocation> m_Characters;

public:
  Font(void);
protected:
  int load(FileStream *file_Stream) override;
};

_GEAR_END