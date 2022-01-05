#pragma once

#include <gear/core/core.h>
#include <gear/resource/Font.h>
#include <gear/math/Vector.h>

_GEAR_START

struct TextComponent {
  bool raw_Text = true;
  const char* text;
  Font *font;
  int width, height;
  Vector<float, 2> offset;
};

_GEAR_END