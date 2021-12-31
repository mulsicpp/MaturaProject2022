#pragma once

#include <gear/core/core.h>
#include "Resource.h"

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
};

_GEAR_END