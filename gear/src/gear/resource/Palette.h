#pragma once

#include <gear/core/core.h>
#include <vector>
#include "Resource.h"

_GEAR_START

class Palette : public Resource {
private:
  uint8_t m_Size;
  uint32_t *m_Colors;
public:
  Palette(void);
  ~Palette();

  Palette(const Palette &) = delete;
  Palette& operator=(const Palette &) = delete;
protected:
  int load(FileStream *file_Stream) override;
};

_GEAR_END