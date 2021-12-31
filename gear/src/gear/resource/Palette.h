#pragma once

#include <gear/core/core.h>
#include <vector>
#include "Resource.h"

_GEAR_START

class Palette : public Resource
{
private:
  uint8_t m_Size;
  uint32_t *m_Colors;

protected:
  int load(FileStream *file_Stream) override;

public:
  Palette(void);
  ~Palette();

  Palette(const Palette &) = delete;
  Palette &operator=(const Palette &) = delete;

  uint8_t get_Size(void) const;
  uint32_t *get_Colors(void) const;
};

_GEAR_END