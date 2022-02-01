#include "animation_player.h"

void gear::animation_Player_Callback(gear::AnimationComponent &animation)
{
  animation.animation_Offset += animation.frame_Rate / 60.0f;
  animation.animation_Offset = int(animation.animation_Offset) % animation.animation->get_Frame_Count() + animation.animation_Offset - int(animation.animation_Offset);
}