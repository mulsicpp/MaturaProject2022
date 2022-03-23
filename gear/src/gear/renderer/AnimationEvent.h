#pragma once

#include <gear/core/core.h>
#include "AnimationType.h"


_GEAR_START

class AnimationEvent {
private:
    float m_Offset;
    float m_Framerate;
    AnimationType m_Type;

public:
    AnimationEvent(float offset, float framerate, AnimationType type);

    float get_Offset(void) const;
    float get_Framerate(void) const;
    AnimationType get_Type(void) const;
};

_GEAR_END