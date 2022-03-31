#include "BaseFighterScript.h"

#include <gear/resource/ResourceManager.h>

#include <gear/collision/DynamicPhysicsComponent.h>
#include <gear/collision/shapes/Rect.h>

using namespace gear;

BaseFighterScript::BaseFighterScript(InputDevice device, const char *base_Path)
{
    input = FighterInput::create_From(device);

    up_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            if (input->special->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("up special");
            else if (input->attack->get_State() == State::PRESSED)
                if (flags & F_GROUND)
                    GEAR_DEBUG_LOG("up ground");
                else
                    GEAR_DEBUG_LOG("up air");
        }
    };

    left_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            if (input->special->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("side special");
            else if (input->attack->get_State() == State::PRESSED)
                if (flags & F_GROUND)
                    GEAR_DEBUG_LOG("side ground");
                else
                    GEAR_DEBUG_LOG("side air");
        }
    };

    right_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            if (input->special->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("side special");
            else if (input->attack->get_State() == State::PRESSED)
                if (flags & F_GROUND)
                    GEAR_DEBUG_LOG("side ground");
                else
                    GEAR_DEBUG_LOG("side air");
        }
    };

    down_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            if (input->special->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("down special");
            else if (input->attack->get_State() == State::PRESSED)
                if (flags & F_GROUND)
                    GEAR_DEBUG_LOG("down ground");
                else
                    GEAR_DEBUG_LOG("down air");
        }
    };

    jump_Callback = [this] (Action a) {
        if(a == Action::PRESSED) {
            auto physics = m_Entity.get<DynamicPhysicsComponent>();
            if(flags & F_GROUND)
                physics->velocity[1] = -this->jump_Strenght;
            else
                physics->velocity[1] = -this->air_Jump_Strength;
        }
    };

    attack_Callback = [this](Action a)
    {
        if (a == Action::PRESSED)
        {
            if (flags & F_GROUND)
            {
                if (input->left->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("side ground");
                else if (input->right->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("side ground");
                else if (input->up->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("up ground");
                else if (input->down->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("down ground");
            }
            else
            {
                if (input->left->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("side air");
                else if (input->right->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("side air");
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
            if (input->left->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("side special");
            else if (input->right->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("side special");
            else if (input->up->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("up special");
            else if (input->down->get_State() == State::PRESSED)
                GEAR_DEBUG_LOG("down special");
        }
    };

    init_Input();
    init_Animations(base_Path);
}

BaseFighterScript::~BaseFighterScript() {}

void BaseFighterScript::init(void)
{
    m_Entity.add<TransformComponent>({{0, 0}, {1, 1}, 0});
    m_Entity.add<AnimationComponent>(a_Idle);
    DynamicPhysicsComponent physics;
    physics.collider = Collider::create(Rect{{-5, -28}, {7, 25}});
    physics.acceleration = {0, 1000};
    m_Entity.add<DynamicPhysicsComponent>(physics);
}

void BaseFighterScript::init_Input(void)
{
    input->up->set_Callback(up_Callback);
    input->right->set_Callback(right_Callback);
    input->down->set_Callback(down_Callback);
    input->left->set_Callback(left_Callback);
    input->jump->set_Callback(jump_Callback);
    input->attack->set_Callback(attack_Callback);
    input->special->set_Callback(special_Callback);
    input->shield->set_Callback(shield_Callback);
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

void BaseFighterScript::pre_Input(void)
{
    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    physics->velocity[0] = 0;
}