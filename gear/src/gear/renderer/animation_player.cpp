#include "animation_player.h"
#include <gear/scene/Entity.h>

#include <gear/core/Game.h>

static std::vector<std::function<void(void)>> callbacks;

static void animation_Player_Callback(gear::Entity parent, gear::AnimationComponent &animation)
{
    int prev_Frame = animation.frame_Offset;
    switch (animation.type)
    {
    case gear::AnimationType::FORWARD:
        if (animation.frame_Offset + animation.frame_Rate * gear::Game::get_Delta_Time() < animation.animation->get_Frame_Count())
            animation.frame_Offset += animation.frame_Rate * gear::Game::get_Delta_Time();
        else if (animation.on_Ended != nullptr)
            callbacks.push_back(animation.on_Ended);
        if (animation.on_Changed != nullptr)
            callbacks.push_back(std::bind(animation.on_Changed, animation.frame_Offset));
        if (animation.on_Frame_Changed != nullptr && (int)animation.frame_Offset != prev_Frame)
            callbacks.push_back(std::bind(animation.on_Frame_Changed, (int)animation.frame_Offset));
        break;
    case gear::AnimationType::LOOP:
        if (animation.frame_Offset + animation.frame_Rate * gear::Game::get_Delta_Time() >= animation.animation->get_Frame_Count())
            if (animation.on_Ended != nullptr)
                callbacks.push_back(animation.on_Ended);
        animation.frame_Offset += animation.frame_Rate * gear::Game::get_Delta_Time();
        animation.frame_Offset = int(animation.frame_Offset) % animation.animation->get_Frame_Count() + animation.frame_Offset - int(animation.frame_Offset);
        if (animation.on_Changed != nullptr)
            callbacks.push_back(std::bind(animation.on_Changed, animation.frame_Offset));
        if (animation.on_Frame_Changed != nullptr && (int)animation.frame_Offset != prev_Frame)
            callbacks.push_back(std::bind(animation.on_Frame_Changed, (int)animation.frame_Offset));
        break;
    case gear::AnimationType::PING_PONG:
        animation.frame_Offset += animation.frame_Rate * gear::Game::get_Delta_Time() * animation.factor;
        while (animation.frame_Offset < 0 || animation.frame_Offset >= animation.animation->get_Frame_Count())
        {
            if (animation.frame_Offset >= animation.animation->get_Frame_Count())
            {
                animation.frame_Offset = 2 * animation.animation->get_Frame_Count() - animation.frame_Offset;
                animation.factor = -1;
                if (animation.on_Ended != nullptr)
                    callbacks.push_back(animation.on_Ended);
            }
            if (animation.frame_Offset < 0)
            {
                animation.frame_Offset = -animation.frame_Offset;
                animation.factor = 1;
                if (animation.on_Ended != nullptr)
                    callbacks.push_back(animation.on_Ended);
            }
        }
        if (animation.on_Changed != nullptr)
            callbacks.push_back(std::bind(animation.on_Changed, animation.frame_Offset));
        if (animation.on_Frame_Changed != nullptr && (int)animation.frame_Offset != prev_Frame)
            callbacks.push_back(std::bind(animation.on_Frame_Changed, (int)animation.frame_Offset));
        break;
    }
}

void gear::continue_Animations(gear::Scene *scene)
{
    gear::Entity::for_Each(scene->get_ID(), animation_Player_Callback);
    for (auto &callback : callbacks)
        callback();
    callbacks.clear();
}