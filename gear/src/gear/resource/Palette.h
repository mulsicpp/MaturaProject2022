#pragma once

#include <gear/core/core.h>
#include "Resource.h"
#include <gear/math/Vector.h>

_GEAR_START

class Palette : public Resource
{
private:
  uint8_t m_Size;
  Vector<float, 4> *m_Colors;

protected:
  int load(FileStream *file_Stream) override;

public:
  Palette(void);
  ~Palette();

  Palette(const Palette &) = delete;
  Palette &operator=(const Palette &) = delete;

  uint8_t get_Size(void) const;
  Vector<float, 4> *get_Colors(void) const;
};

_GEAR_END