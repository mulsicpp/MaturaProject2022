#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include <gear/resource/Sprite.h>
#include <gear/resource/Palette.h>

_GEAR_START

struct SpriteComponent {
  Ref<Sprite> sprite;
  Ref<Palette> palette;
  float parallax_Factor = 1;
  Vector<float, 3> offset = { 0, 0 };
};

_GEAR_END