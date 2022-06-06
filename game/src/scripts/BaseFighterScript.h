#pragma once

#include <gear/scripting/ScriptableEntity.h>

#include <gear/collision/Hitbox.h>
#include <gear/collision/Hurtbox.h>
#include <gear/collision/Collider.h>

#include <gear/collision/DynamicPhysicsComponent.h>

#include <gear/renderer/AnimationComponent.h>

#include "../input/FighterInput.h"

//#include <gear/scene/Entity.h>

#define FIGHTER_GROUND 0x01
#define FIGHTER_INPUT_BLOCKED 0x02
#define FIGHTER_PERFORMING_ACTION 0x04

typedef uint8_t state_t;

state_t new_State(void);

#define FIGHTER_STATE(x) const state_t x = new_State();

FIGHTER_STATE(FIGHTER_IDLE)
FIGHTER_STATE(FIGHTER_SHIELD)
FIGHTER_STATE(FIGHTER_SIDE_SPECIAL)
FIGHTER_STATE(FIGHTER_UP_SPECIAL)
FIGHTER_STATE(FIGHTER_DOWN_SPECIAL)
FIGHTER_STATE(FIGHTER_SIDE_GROUND)
FIGHTER_STATE(FIGHTER_UP_GROUND)
FIGHTER_STATE(FIGHTER_DOWN_GROUND)
FIGHTER_STATE(FIGHTER_SIDE_AIR)
FIGHTER_STATE(FIGHTER_UP_AIR)
FIGHTER_STATE(FIGHTER_DOWN_AIR)
FIGHTER_STATE(FIGHTER_LEDGE_GRAB)
FIGHTER_STATE(FIGHTER_HANGING)
FIGHTER_STATE(FIGHTER_GET_UP)

class BaseFighterScript : public gear::ScriptableEntity
{
private:
    state_t m_State;
    gear::AnimationComponent *m_Running_Animation;

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

    int max_Health = 500;
    int health = 500;

    gear::Entity health_Display;

    // std::function<void(gear::Action)>
    //     up_Callback,
    //     down_Callback,
    //     jump_Callback,
    //     attack_Callback,
    //     special_Callback,
    //     shield_Callback;

    // std::function<void(float)> x_Callback;

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
    
    std::unordered_map<state_t, gear::AnimationComponent*> animations;

    int player_Number = 0;

public:
    BaseFighterScript(gear::InputDevice device, const char *base_Path, std::string palette_Name, int player_Number);
    ~BaseFighterScript();

    virtual void init(void) override;

    virtual void init_Input(void);
    virtual void init_Animations(const char *base_Path, std::string palette_Name);
    virtual void map_Animations(void);

    void init_Animation(gear::AnimationComponent *animation, std::string path, gear::Ref<gear::Palette> palette);

    static int axis_As_Int(float value);

    virtual void pre_Physics(void) override;
    virtual void pre_Render(void) override;

    virtual void init_Animation_Events(void);

    virtual void damage(int damage);
    virtual void respawn(void);

    void return_To_Idle(void);

    // virtual void slow(double duration);
    // virtual void stun(double duration);

    // virtual void reset_Hitboxes(void);

    virtual void play_Animation(gear::AnimationComponent *animation, bool force = false);
    virtual void end_Animation(void);

    void set_Direction(int dir);

    virtual bool is_Phasing(void);

    void set_State(state_t new_State, bool force = false);
    state_t get_State(void) const;

    virtual void on_State_Switch(state_t new_State, state_t old_State);

    void x_Callback(float value);
    void up_Callback(gear::Action a);
    void down_Callback(gear::Action a);
    void jump_Callback(gear::Action a);
    void attack_Callback(gear::Action a);
    void special_Callback(gear::Action a);
    void shield_Callback(gear::Action a);
};