#pragma once

#include <gear/core/core.h>
#include "Resource.h"

_GEAR_START

class Animation : public Resource
{
private:
  uint16_t m_Width, m_Height, m_Frame_Count;
  float m_Default_Frame_Rate;
  unsigned int m_TextureID;

protected:
  int load(FileStream *file_Stream) override;

public:
  Animation(void);
  ~Animation();

  Animation(const Animation &) = delete;
  Animation &operator=(const Animation &) = delete;
};

_GEAR_END