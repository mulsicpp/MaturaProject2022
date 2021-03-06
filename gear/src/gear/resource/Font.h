#pragma once

#include <gear/core/core.h>
#include <stdint.h>
#include <unordered_map>
#include <gear/data/Grid.h>
#include "Resource.h"

#include <vector>
#include <gear/math/Vector.h>

_GEAR_START

class Font : public Resource
{
public:
  struct CharacterLocation
  {
    uint16_t x_Start;
    uint16_t x_End;
    uint16_t y_Start;
    uint16_t y_End;
  };
  
private:
  uint8_t m_Flags;
  int16_t m_Char_Gap;
  int16_t m_Line_Gap;
  int16_t m_Space_Gap;
  uint16_t m_Width, m_Height;
  unsigned int m_TextureID;
  std::unordered_map<char, CharacterLocation> m_Characters;
  std::vector<Vector<uint8_t, 4>> m_Colors;

protected:
  int load(FileStream *file_Stream) override;

public:
  Font(void);
  ~Font();

  Font(const Font &) = delete;
  Font &operator=(const Font &) = delete;

  uint8_t get_Flags(void) const;
  int16_t get_Char_Gap(void) const;
  int16_t get_Line_Gap(void) const;
  int16_t get_Space_Gap(void) const;
  uint16_t get_Width(void) const;
  uint16_t get_Height(void) const;
  unsigned int get_TextureID(void) const;
  const CharacterLocation *get_Char(unsigned int character) const;

  const std::vector<Vector<uint8_t, 4>> &get_Colors(void) const;
};

_GEAR_END