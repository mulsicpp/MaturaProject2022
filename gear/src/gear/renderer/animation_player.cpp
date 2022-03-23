#include "animation_player.h"

void gear::animation_Player_Callback(gear::AnimationComponent &animation)
{
  animation.frame_Offset += animation.frame_Rate / 60.0f;
  animation.frame_Offset = int(animation.frame_Offset) % animation.animation->get_Frame_Count() + animation.frame_Offset - int(animation.frame_Offset);
}