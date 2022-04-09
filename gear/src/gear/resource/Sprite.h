#pragma once

#include <gear/core/core.h>
#include "Resource.h"

#include "Font.h"

_GEAR_START

class Sprite : public Resource
{
private:
  uint16_t m_Width, m_Height;
  unsigned int m_TextureID;

protected:
  int load(FileStream *file_Stream) override;

public:
  Sprite(void);
  ~Sprite();

  Sprite(const Sprite &) = delete;
  Sprite &operator=(const Sprite &) = delete;

  uint16_t get_Width(void) const;
  uint16_t get_Height(void) const;

  unsigned int get_TextureID(void) const;

  static Ref<Sprite> from_Font(Ref<Font> font);
};

_GEAR_END