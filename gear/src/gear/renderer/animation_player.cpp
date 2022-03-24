#include "animation_player.h"
#include <gear/scene/Entity.h>

#include <gear/core/Game.h>

static std::vector<std::function<void(void)>> callbacks;

void gear::animation_Player_Callback(gear::AnimationComponent &animation)
{
    switch (animation.type)
    {
    case AnimationType::FORWARD:
        if (animation.frame_Offset + animation.frame_Rate * gear::Game::get_Delta_Time() < animation.animation->get_Frame_Count())
            animation.frame_Offset += animation.frame_Rate * gear::Game::get_Delta_Time();
        else
            if(animation.on_End != nullptr) callbacks.push_back(std::bind(animation.on_End, AnimationEvent(animation.frame_Offset, animation.frame_Rate, animation.type)));
        if(animation.on_Change != nullptr) callbacks.push_back(std::bind(animation.on_Change, AnimationEvent(animation.frame_Offset, animation.frame_Rate, animation.type)));
        break;
    case AnimationType::LOOP:
        if (animation.frame_Offset + animation.frame_Rate * gear::Game::get_Delta_Time() >= animation.animation->get_Frame_Count())
            if(animation.on_End != nullptr) callbacks.push_back(std::bind(animation.on_End, AnimationEvent(animation.frame_Offset, animation.frame_Rate, animation.type)));
        animation.frame_Offset += animation.frame_Rate * gear::Game::get_Delta_Time();
        animation.frame_Offset = int(animation.frame_Offset) % animation.animation->get_Frame_Count() + animation.frame_Offset - int(animation.frame_Offset);
        if(animation.on_Change != nullptr) callbacks.push_back(std::bind(animation.on_Change, AnimationEvent(animation.frame_Offset, animation.frame_Rate, animation.type)));
        break;
    case AnimationType::PING_PONG:
        animation.frame_Offset += animation.frame_Rate * gear::Game::get_Delta_Time() * animation.factor;
        while (animation.frame_Offset < 0 || animation.frame_Offset >= animation.animation->get_Frame_Count())
        {
            if (animation.frame_Offset >= animation.animation->get_Frame_Count())
            {
                animation.frame_Offset = 2 * animation.animation->get_Frame_Count() - animation.frame_Offset;
                animation.factor = -1;
                if(animation.on_End != nullptr) callbacks.push_back(std::bind(animation.on_End, AnimationEvent(animation.frame_Offset, animation.frame_Rate, animation.type)));
            }
            if (animation.frame_Offset < 0)
            {
                animation.frame_Offset = -animation.frame_Offset;
                animation.factor = 1;
                if(animation.on_End != nullptr) callbacks.push_back(std::bind(animation.on_End, AnimationEvent(animation.frame_Offset, animation.frame_Rate, animation.type)));
            }
        }
        if(animation.on_Change != nullptr) callbacks.push_back(std::bind(animation.on_Change, AnimationEvent(animation.frame_Offset, animation.frame_Rate, animation.type)));
        break;
    }
}

void gear::continue_Animations(gear::Scene *scene)
{
    gear::Entity::for_Each(scene->get_ID(), gear::animation_Player_Callback);
    for(auto &callback : callbacks)
        callback();
    callbacks.clear();
}