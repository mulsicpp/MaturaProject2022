#pragma once

#include <gear/core/core.h>
#include <gear/data/Grid.h>
#include "Palette.h"

_GEAR_START

class Image {
private:
  unsigned int m_Width, m_Height;
  unsigned int m_Texture;
  Palette *m_Palette;
};

_GEAR_END