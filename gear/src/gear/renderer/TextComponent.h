#pragma once

#include <gear/core/core.h>
#include <gear/resource/Font.h>
#include <gear/math/Vector.h>

#include <vector>

#include <gear/collision/shapes/Rect.h>

_GEAR_START

struct TextComponent {
  bool raw_Text = true;
  const char* text;
  Ref<Font> font;
  Vector<double, 3> offset = {0, 0, 0};
  unsigned int width, height;
};

_GEAR_END