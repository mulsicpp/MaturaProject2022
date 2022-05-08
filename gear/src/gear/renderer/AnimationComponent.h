#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include <gear/resource/Animation.h>
#include <gear/resource/Palette.h>
#include <functional>

_GEAR_START

struct AnimationComponent {
  Ref<Animation> animation;
  Ref<Palette> palette;
  float frame_Offset;
  float frame_Rate;
  float parallax_Factor = 1;
  gear::Vector<float, 3> offset = { 0, 0, 0 };
  float factor = 1;

  std::function<void (float)> on_Changed;
  std::function<void (void)> on_Ended;
  std::function<void (int)> on_Frame_Changed;
};

_GEAR_END