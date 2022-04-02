#include "BaseFighterScript.h"

#include <gear/resource/ResourceManager.h>

#include <gear/collision/shapes/Rect.h>

#include "../components/FlagComponent.h"

using namespace gear;

static bool fighter_Physics_Check(gear::CollisionEvent e)
{
    if (e.get_Other_Entity().get<FlagComponent>()->flags & FLAG_FIGHTER)
        return false;
    return true;
}

BaseFighterScript::BaseFighterScript(InputDevice device, const char *base_Path)
{
    input = FighterInput::create_From(device);

    x_Callback = [this](float val)
    {
        static int prev_Val = 0;
        int real_Val = axis_As_Int(val);
        if (prev_Val != real_Val)
        {
            if (real_Val)
            {
                auto transform = m_Entity.get<TransformComponent>();
                transform->scale[0] = real_Val;
                m_Entity.update_Transformation();
                play_Animation(&a_Run);
                if (input->special->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("side special %i", real_Val);
                else if (input->attack->get_State() == State::PRESSED)
                    if (flags & FIGHTER_GROUND)
                        GEAR_DEBUG_LOG("side ground %i", real_Val);
                    else
                        GEAR_DEBUG_LOG("side air %i", real_Val);
            } else {
                play_Animation(&a_Idle);
            }
        }
        prev_Val = val;
    };

    up_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            if (input->special->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("up special");
            else if (input->attack->get_State() == State::PRESSED)
                if (flags & FIGHTER_GROUND)
                    GEAR_DEBUG_LOG("up ground");
                else
                    GEAR_DEBUG_LOG("up air");
        }
    };

    down_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            if (input->special->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("down special");
            else if (input->attack->get_State() == State::PRESSED)
                if (flags & FIGHTER_GROUND)
                    GEAR_DEBUG_LOG("down ground");
                else
                    GEAR_DEBUG_LOG("down air");
        }
    };

    jump_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            auto physics = m_Entity.get<DynamicPhysicsComponent>();
            if (flags & FIGHTER_GROUND)
                physics->velocity[1] = -this->jump_Strenght;
            else if(air_Jumps > 0) {
                physics->velocity[1] = -this->air_Jump_Strength;
                air_Jumps--;
            }
        }
    };

    attack_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            int val = axis_As_Int(input->x_Axis->get_Value());
            if (flags & FIGHTER_GROUND)
            {
                if (val)
                    GEAR_DEBUG_LOG("side ground %i", val);
                else if (input->up->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("up ground");
                else if (input->down->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("down ground");
            }
            else
            {
                if (val)
                    GEAR_DEBUG_LOG("side air %i", val);
                else if (input->up->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("up air");
                else if (input->down->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("down air");
            }
        }
    };

    special_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            int val = axis_As_Int(input->x_Axis->get_Value());
            if (val)
                GEAR_DEBUG_LOG("side special %i", val);
            else if (input->up->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("up special");
            else if (input->down->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("down special");
        }
    };

    physics.acceleration = {0, 1000};
    physics.velocity_Y_Interval[1] = 300;
    physics.on_Collision = [this](CollisionEvent e)
    {
        if (abs(e.get_Separation_Vector()[0]) < abs(e.get_Separation_Vector()[1]) / 20 && e.get_Separation_Vector()[1] > 0)
        {
            flags |= FIGHTER_GROUND;
            air_Jumps = max_Air_Jumps;
        }
    };
    physics.check = fighter_Physics_Check;

    init_Input();
    init_Animations(base_Path);
}

BaseFighterScript::~BaseFighterScript() {}

void BaseFighterScript::init(void)
{
    m_Entity.set<FlagComponent>({FLAG_FIGHTER});
    m_Entity.add<AnimationComponent>(a_Idle);
    m_Entity.add<DynamicPhysicsComponent>(physics);
}

void BaseFighterScript::init_Input(void)
{
    input->up->set_Callback(up_Callback);
    input->down->set_Callback(down_Callback);
    input->jump->set_Callback(jump_Callback);
    input->attack->set_Callback(attack_Callback);
    input->special->set_Callback(special_Callback);
    input->shield->set_Callback(shield_Callback);

    input->x_Axis->set_Callback(x_Callback);
}

static void init_Animation(AnimationComponent *animation, std::string path, Ref<Palette> palette, AnimationType type)
{

    animation->animation = ResourceManager::get<Animation>(path);
    animation->palette = palette;
    animation->type = type;

    animation->offset = {-40, -40, 0};
    animation->parallax_Factor = 1;
    animation->frame_Offset = 0;
    animation->frame_Rate = animation->animation->get_Default_Frame_Rate();
}

void BaseFighterScript::init_Animations(const char *base_Path)
{
    std::string path = base_Path;
    Ref<Palette> palette = ResourceManager::get<Palette>(path + "/palettes/default.gear");

    init_Animation(&a_Idle, path + "/animations/idle.gear", palette, AnimationType::LOOP);
    init_Animation(&a_Run, path + "/animations/run.gear", palette, AnimationType::LOOP);
    init_Animation(&a_Jump, path + "/animations/jump.gear", palette, AnimationType::FORWARD);

    // init_Animation(&a_Sground, path + "/animations/sground.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Uground, path + "/animations/uground.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Dground, path + "/animations/dground.gear", palette, AnimationType::FORWARD);

    // init_Animation(&a_Sair, path + "/animations/sair.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Uair, path + "/animations/uair.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Dair, path + "/animations/dair.gear", palette, AnimationType::FORWARD);

    // init_Animation(&a_Sspecial, path + "/animations/sspecial.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Uspecial, path + "/animations/uspecial.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Dspecial, path + "/animations/dspecial.gear", palette, AnimationType::FORWARD);

    // init_Animation(&a_Ult, path + "/ult.gear", palette, AnimationType::FORWARD);

    // init_Animation(&a_Damaged, path + "/damaged.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Shield, path + "/shield.gear", palette, AnimationType::LOOP);

    // init_Animation(&a_Ledge_Grab, path + "/ledge_grab.gear", palette, AnimationType::FORWARD);
    // init_Animation(&a_Hanging, path + "/hanging.gear", palette, AnimationType::LOOP);
    // init_Animation(&a_Get_Up, path + "/get_up.gear", palette, AnimationType::FORWARD);
}

int BaseFighterScript::axis_As_Int(float value)
{
    return value > 0.3 ? 1 : (value < -0.3 ? -1 : 0);
}

void BaseFighterScript::pre_Physics(void)
{
    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    physics->velocity[0] = (flags & FIGHTER_GROUND ? 1 : air_Movement_Factor) * movement_Speed * axis_As_Int(input->x_Axis->get_Value());

    flags &= ~FIGHTER_GROUND;
}

void BaseFighterScript::play_Animation(AnimationComponent *animation)
{
    m_Entity.set<AnimationComponent>(*animation);
}

bool BaseFighterScript::is_Phasing(void) {
    return input->down->get_State() == State::PRESSED;
}