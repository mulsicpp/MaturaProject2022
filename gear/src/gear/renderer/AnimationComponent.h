#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include <gear/resource/Animation.h>
#include <gear/resource/Palette.h>
#include <functional>

#include "AnimationEvent.h"

_GEAR_START

struct AnimationComponent {
  Ref<Animation> animation;
  Ref<Palette> palette;
  AnimationType type = AnimationType::LOOP;
  float frame_Offset;
  float frame_Rate;
  float parallax_Factor = 1;
  gear::Vector<float, 3> offset = { 0, 0, 0 };

  std::function<void (AnimationEvent)> on_Change;
  std::function<void (AnimationEvent)> on_End;
};

_GEAR_END