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
  Rect bounding_Box;
};

_GEAR_END