#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include <gear/resource/Animation.h>
#include <gear/resource/Palette.h>

_GEAR_START

struct AnimationComponent {
  Ref<Animation> animation;
  Ref<Palette> palette;
  float animation_Offset;
  float frame_Rate;
  float parallax_Factor = 1;
  gear::Vector<float, 3> offset;
};

_GEAR_END