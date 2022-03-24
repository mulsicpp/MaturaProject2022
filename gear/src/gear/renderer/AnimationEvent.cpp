#include "AnimationEvent.h"

gear::AnimationEvent::AnimationEvent(float offset, float frame_Rate, gear::AnimationType type)
    : m_Offset(offset), m_Frame_Rate(frame_Rate), m_Type(type)
{
}

float gear::AnimationEvent::get_Offset(void) const { return m_Offset; }

float gear::AnimationEvent::get_Frame_Rate(void) const { return m_Frame_Rate; }

gear::AnimationType gear::AnimationEvent::get_Type(void) const { return m_Type; }