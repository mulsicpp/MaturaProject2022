#pragma once

#include <gear/scripting/ScriptableEntity.h>

#include <gear/collision/Hitbox.h>
#include <gear/collision/Hurtbox.h>
#include <gear/collision/Collider.h>

#include <gear/collision/DynamicPhysicsComponent.h>

#include <gear/renderer/AnimationComponent.h>

#include "../input/FighterInput.h"

#define FIGHTER_GROUND 0x01
#define FIGHTER_INPUT_BLOCKED 0x02
#define FIGHTER_PERFORMING_ACTION 0x04

#define FIGHTER_ANIMATION_DEFAULT 0
#define FIGHTER_ANIMATION_ATTACK 1
#define FIGHTER_ANIMATION_SPECIAL 2

class BaseFighterScript : public gear::ScriptableEntity
{
protected:
    static gear::Ref<gear::Animation> error_Animation;
    static gear::Ref<gear::Palette> error_Palette;

    gear::Ref<FighterInput> input;

    gear::Ref<gear::Collider> collider;
    gear::Ref<gear::Hurtbox> hurtbox;
    uint64_t flags = 0, prev_Flags = 0;

    int air_Jumps = 1;
    int max_Air_Jumps = 1;
    double movement_Speed = 170;
    double air_Movement_Factor = 0.8;
    double jump_Strenght = 400;
    double air_Jump_Strength = 350;

    std::function<void(gear::Action)>
        up_Callback,
        down_Callback,
        jump_Callback,
        attack_Callback,
        special_Callback,
        shield_Callback;

    std::function<void(float)> x_Callback;

    gear::DynamicPhysicsComponent physics;

    gear::Ref<gear::Palette> palette;

    gear::AnimationComponent
        a_Idle[2],
        a_Run[2],
        a_Jump[2],
        a_Sground[2],
        a_Uground[2],
        a_Dground[2],
        a_Sair[2],
        a_Uair[2],
        a_Dair[2],
        a_Sspecial[2],
        a_Uspecial[2],
        a_Dspecial[2],
        a_Ult[2],
        a_Damaged[2],
        a_Shield[2],
        a_Ledge_Grab[2],
        a_Hanging[2],
        a_Get_Up[2];

public:
    BaseFighterScript(gear::InputDevice device, const char *base_Path);
    ~BaseFighterScript();

    virtual void init(void) override;

    virtual void init_Input(void);
    virtual void init_Animations(const char *base_Path);

    void init_Animation(gear::AnimationComponent *animation, std::string path, gear::Ref<gear::Palette> palette);

    static int axis_As_Int(float value);

    virtual void pre_Physics(void) override;
    virtual void init_Animation_Events(void) = 0;

    // virtual void damage(double damage);

    // virtual void slow(double duration);
    // virtual void stun(double duration);

    // virtual void reset_Hitboxes(void);

    virtual void play_Animation(gear::AnimationComponent *animation, uint8_t type = FIGHTER_ANIMATION_DEFAULT);
    virtual void end_Animation(void);

    void set_Direction(int dir);

    virtual bool is_Phasing(void);
};