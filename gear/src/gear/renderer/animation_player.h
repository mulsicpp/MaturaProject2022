#pragma once

#include <gear/core/core.h>
#include "AnimationComponent.h"

#include <gear/scene/Scene.h>

_GEAR_START

void animation_Player_Callback(AnimationComponent &animation);

void continue_Animations(Scene *scene);

_GEAR_END