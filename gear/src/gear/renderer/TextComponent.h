#pragma once

#include <gear/core/core.h>
#include <gear/resource/Font.h>
#include <gear/math/Vector.h>

#include <vector>
#include <string>

#include <gear/collision/shapes/Rect.h>

_GEAR_START

struct TextComponent {
  bool raw_Text = true;
  std::string text;
  Ref<Font> font;
  Vector<double, 3> offset = {0, 0, 0};
  unsigned int width, height;
  std::vector<gear::Vector<uint8_t, 4>> colors;
};

_GEAR_END