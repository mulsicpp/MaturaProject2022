#include <gear/scripting/ScriptableEntity.h>

#include <gear/collision/Hitbox.h>
#include <gear/collision/Hurtbox.h>
#include <gear/collision/Collider.h>

#include <gear/renderer/AnimationComponent.h>

#include "../input/FighterInput.h"

#define F_GROUND 0x01
#define F_RECEIVE_INPUT 0x02
#define F_USED_SPECIAL_MOVE 0x04

class BaseFighterScript : public gear::ScriptableEntity
{

protected:
    gear::Ref<FighterInput> input;

    gear::Ref<gear::Collider> collider;
    gear::Ref<gear::Hurtbox> hurtbox;
    uint64_t flags;

    int air_Jumps = 1;
    double movement_Speed = 170;
    double air_Movement_Factor = 0.5;
    double jump_Strenght = 400;
    double air_Jump_Strength = 300;

    gear::AnimationComponent
        a_Idle,
        a_Run,
        a_Jump,
        a_Sground,
        a_Uground,
        a_Dground,
        a_Sspecial,
        a_Uspecial,
        a_Dspecial,
        a_Sair,
        a_Uair,
        a_Dair,
        a_Ult,
        a_Damaged,
        a_Ledge_Grab,
        a_Get_Up,
        a_Shield;

public:
    BaseFighterScript(gear::InputDevice device);
    ~BaseFighterScript();

    virtual void init_Input(void);
    // virtual void init_Animations(const char *base_Path);
    // virtual void init_Animation_Events(void);

    // virtual void damage(double damage);

    // virtual void slow(double duration);
    // virtual void stun(double duration);

    // virtual void reset_Hitboxes(void);

    // virtual void play_Animation(AnimationComponent animation);
};