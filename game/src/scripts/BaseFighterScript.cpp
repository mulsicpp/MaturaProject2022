#include "BaseFighterScript.h"

#include <gear/resource/ResourceManager.h>
#include <gear/resource/Font.h>

#include <gear/collision/shapes/Rect.h>

#include <gear/renderer/TextComponent.h>

#include "../components/FlagComponent.h"

static state_t next_State = 0;

using namespace gear;

state_t new_State(void)
{
    return next_State++;
}

gear::Ref<gear::Animation> BaseFighterScript::error_Animation = nullptr;
gear::Ref<gear::Palette> BaseFighterScript::error_Palette = nullptr;

static bool fighter_Physics_Check(gear::CollisionEvent e)
{
    if (e.get_Other_Entity().get<FlagComponent>()->flags & FLAG_FIGHTER)
        return false;
    return true;
}

BaseFighterScript::BaseFighterScript(InputDevice device, const char *base_Path, std::string palette_Name, int player_Number) : player_Number(player_Number)
{
    if (!error_Animation)
        error_Animation = ResourceManager::get<Animation>("assets/fighters/_error/error.gear");
    if (!error_Palette)
        error_Palette = ResourceManager::get<Palette>("assets/fighters/_error/error_palette.gear");

    input = FighterInput::create_From(device);

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

    init_Animations(base_Path, palette_Name);
}

BaseFighterScript::~BaseFighterScript() {}

void BaseFighterScript::x_Callback(float val)
{
    static int prev_Val = 0;
    int real_Val = axis_As_Int(val);
    if (prev_Val != real_Val)
    {
        if (real_Val)
        {
            set_Direction(real_Val);
            play_Animation(&a_Run[0]);
            if (input->special->get_State() == State::PRESSED)
                play_Animation(a_Sspecial, FIGHTER_ANIMATION_SPECIAL);
            else if (input->attack->get_State() == State::PRESSED)
                if (flags & FIGHTER_GROUND)
                    play_Animation(a_Sground, FIGHTER_ANIMATION_ATTACK);
                else
                    play_Animation(a_Sair, FIGHTER_ANIMATION_ATTACK);
        }
        else if (flags & FIGHTER_GROUND)
        {
            play_Animation(&a_Idle[0]);
        }
    }
    prev_Val = val;
};

void BaseFighterScript::up_Callback(Action a)
{
    if (a == Action::PRESSED)
    {
        if (input->special->get_State() == State::PRESSED)
            play_Animation(a_Uspecial, FIGHTER_ANIMATION_SPECIAL);
        else if (input->attack->get_State() == State::PRESSED)
            if (flags & FIGHTER_GROUND)
                play_Animation(a_Uground, FIGHTER_ANIMATION_ATTACK);
            else
                play_Animation(a_Uair, FIGHTER_ANIMATION_ATTACK);
    }
};

void BaseFighterScript::down_Callback(Action a)
{
    if (a == Action::PRESSED)
    {
        if (input->special->get_State() == State::PRESSED)
            play_Animation(a_Dspecial, FIGHTER_ANIMATION_SPECIAL);
        else if (input->attack->get_State() == State::PRESSED)
            if (flags & FIGHTER_GROUND)
                play_Animation(a_Dground, FIGHTER_ANIMATION_ATTACK);
            else
                play_Animation(a_Dair, FIGHTER_ANIMATION_ATTACK);
    }
};

void BaseFighterScript::jump_Callback(Action a)
{
    if (a == Action::PRESSED)
    {
        auto physics = m_Entity.get<DynamicPhysicsComponent>();
        if (flags & FIGHTER_GROUND)
            physics->velocity[1] = -this->jump_Strenght;
        else if (air_Jumps > 0)
        {
            physics->velocity[1] = -this->air_Jump_Strength;
            air_Jumps--;
        }
    }
};

void BaseFighterScript::attack_Callback(Action a)
{
    if (a == Action::PRESSED)
    {
        int val = axis_As_Int(input->x_Axis->get_Value());
        if (flags & FIGHTER_GROUND)
        {
            if (val)
            {
                set_Direction(val);
                play_Animation(a_Sground, FIGHTER_ANIMATION_ATTACK);
                GEAR_DEBUG_LOG("side ground %i", val);
            }
            else if (input->up->get_State() == State::PRESSED)
            {
                play_Animation(a_Uground, FIGHTER_ANIMATION_ATTACK);
                GEAR_DEBUG_LOG("up ground");
            }
            else if (input->down->get_State() == State::PRESSED)
            {
                play_Animation(a_Dground, FIGHTER_ANIMATION_ATTACK);
                GEAR_DEBUG_LOG("down ground");
            }
        }
        else
        {
            if (val)
            {
                set_Direction(val);
                play_Animation(a_Sair, FIGHTER_ANIMATION_ATTACK);
                GEAR_DEBUG_LOG("side air %i", val);
            }
            else if (input->up->get_State() == State::PRESSED)
            {
                play_Animation(a_Uair, FIGHTER_ANIMATION_ATTACK);
                GEAR_DEBUG_LOG("up air");
            }
            else if (input->down->get_State() == State::PRESSED)
            {
                play_Animation(a_Dair, FIGHTER_ANIMATION_ATTACK);
                GEAR_DEBUG_LOG("down air");
            }
        }
    }
};

void BaseFighterScript::special_Callback(Action a)
{
    if (a == Action::PRESSED)
    {
        int val = axis_As_Int(input->x_Axis->get_Value());
        if (val)
        {
            set_Direction(val);
            play_Animation(a_Sspecial, FIGHTER_ANIMATION_SPECIAL);
            GEAR_DEBUG_LOG("side special %i", val);
        }
        else if (input->up->get_State() == State::PRESSED)
        {
            play_Animation(a_Uspecial, FIGHTER_ANIMATION_SPECIAL);
            GEAR_DEBUG_LOG("up special");
        }
        else if (input->down->get_State() == State::PRESSED)
        {
            play_Animation(a_Dspecial, FIGHTER_ANIMATION_SPECIAL);
            GEAR_DEBUG_LOG("down special");
        }
    }
}

void BaseFighterScript::shield_Callback(Action a)
{
}

void BaseFighterScript::init(void)
{
    init_Input();
    init_Animation_Events();

    m_Entity.set<FlagComponent>({FLAG_FIGHTER});
    m_Entity.add<AnimationComponent>(a_Idle[0]);
    m_Entity.add<DynamicPhysicsComponent>(physics);

    health_Display = Scene::get(m_Entity.get_Scene_ID())->create_Entity();

    TextComponent t;
    t.text = "";
    t.font = ResourceManager::get<Font>("assets/fonts/font1.gear");
    t.colors = std::vector<Vector<uint8_t, 4>>(t.font->get_Colors());
    switch (player_Number)
    {
    case 0:
        t.colors[0] = {255, 32, 32, 255};
        t.colors[1] = {127, 16, 16, 255};
        break;
    case 1:
        t.colors[0] = {32, 64, 255, 255};
        t.colors[1] = {16, 32, 127, 255};
        break;
    }
    t.break_Word = true;
    t.height = 200;
    t.width = 200;
    health_Display.add<TextComponent>(t);
}

void BaseFighterScript::init_Input(void)
{
    input->up->set_Callback([this] (Action a) { this->up_Callback(a); });
    input->down->set_Callback([this] (Action a) { this->down_Callback(a); });
    input->jump->set_Callback([this] (Action a) { this->jump_Callback(a); });
    input->attack->set_Callback([this] (Action a) { this->attack_Callback(a); });
    input->special->set_Callback([this] (Action a) { this->special_Callback(a); });
    input->shield->set_Callback([this] (Action a) { this->shield_Callback(a); });

    input->x_Axis->set_Callback([this] (float v) { this->x_Callback(v); });
}

void BaseFighterScript::init_Animation(AnimationComponent *animation, std::string path, Ref<Palette> palette)
{
    Ref<Animation> a = ResourceManager::get<Animation>(path);
    if (a.get())
    {
        animation->animation = a;
        animation->palette = palette;
    }
    else
    {
        animation->animation = error_Animation;
        animation->palette = error_Palette;
    }

    animation->offset = {-40, -40, 0};
    animation->parallax_Factor = 1;
    animation->frame_Offset = 0;
    animation->frame_Rate = animation->animation->get_Frame_Rate();

    animation->on_Ended = [this](void)
    {
        flags &= ~FIGHTER_PERFORMING_ACTION;
        input->set_Enabled_All(true);
        end_Animation();
    };
}

void BaseFighterScript::init_Animations(const char *base_Path, std::string palette_Name)
{
    std::string path = base_Path;
    palette = ResourceManager::get<Palette>(path + "/palettes/" + palette_Name);

    init_Animation(&a_Idle[0], path + "/animations/right/idle.gear", palette);
    init_Animation(&a_Run[0], path + "/animations/right/run.gear", palette);
    init_Animation(&a_Jump[0], path + "/animations/right/jump.gear", palette);

    init_Animation(&a_Sground[0], path + "/animations/right/sground.gear", palette);
    init_Animation(&a_Uground[0], path + "/animations/right/uground.gear", palette);
    init_Animation(&a_Dground[0], path + "/animations/right/dground.gear", palette);

    init_Animation(&a_Sair[0], path + "/animations/right/sair.gear", palette);
    init_Animation(&a_Uair[0], path + "/animations/right/uair.gear", palette);
    init_Animation(&a_Dair[0], path + "/animations/right/dair.gear", palette);

    init_Animation(&a_Sspecial[0], path + "/animations/right/sspecial.gear", palette);
    init_Animation(&a_Uspecial[0], path + "/animations/right/uspecial.gear", palette);
    init_Animation(&a_Dspecial[0], path + "/animations/right/dspecial.gear", palette);

    init_Animation(&a_Ult[0], path + "/animations/right/ult.gear", palette);

    init_Animation(&a_Damaged[0], path + "/animations/right/damaged.gear", palette);
    init_Animation(&a_Shield[0], path + "/animations/right/shield.gear", palette);

    init_Animation(&a_Ledge_Grab[0], path + "/animations/right/ledge_grab.gear", palette);
    init_Animation(&a_Hanging[0], path + "/animations/right/hanging.gear", palette);
    init_Animation(&a_Get_Up[0], path + "/animations/right/get_up.gear", palette);
}

void BaseFighterScript::init_Animation_Events(void) {}

int BaseFighterScript::axis_As_Int(float value)
{
    return value > 0.3 ? 1 : (value < -0.3 ? -1 : 0);
}

void BaseFighterScript::damage(int amount)
{
    health -= amount;
    if (health <= 0)
        respawn();
}

void BaseFighterScript::respawn(void)
{
    m_Entity.get<TransformComponent>()->position = {0, 0};
    health = max_Health;
}

void BaseFighterScript::pre_Physics(void)
{
    auto transform = m_Entity.get<TransformComponent>();
    if (transform->position[1] > 400)
    {
        respawn();
    }

    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    physics->velocity[0] = (flags & FIGHTER_GROUND ? 1 : air_Movement_Factor) * movement_Speed * axis_As_Int(input->x_Axis->get_Value());

    // GEAR_DEBUG_LOG("flags: %x", flags);
    if ((flags & FIGHTER_PERFORMING_ACTION) == 0)
    {
        if ((flags & FIGHTER_GROUND) == 0)
        {
            if (physics->velocity[1] < -300)
                a_Jump[0].frame_Offset = 1;
            else if (physics->velocity[1] < -100)
                a_Jump[0].frame_Offset = 2;
            else if (physics->velocity[1] < 100)
                a_Jump[0].frame_Offset = 3;
            else if (physics->velocity[1] < 300)
                a_Jump[0].frame_Offset = 4;
            else
                a_Jump[0].frame_Offset = 5;
            m_Entity.set<AnimationComponent>(a_Jump[0]);
        }
        else
        {
            if ((prev_Flags & FIGHTER_GROUND) == 0)
            {
                if (axis_As_Int(input->x_Axis->get_Value()) == 0)
                    m_Entity.set<AnimationComponent>(a_Idle[0]);
                else
                {
                    m_Entity.set<AnimationComponent>(a_Run[0]);
                }
            }
        }
    }

    prev_Flags = flags;
    flags &= ~FIGHTER_GROUND;
}

void BaseFighterScript::pre_Render(void)
{
    health_Display.get<TransformComponent>()->position = m_Entity.get<TransformComponent>()->position - Vector<double, 2>{10, 20};
    health_Display.update_Transformation();

    char buffer[20];
    sprintf(buffer, "%i/%i", health, max_Health);

    health_Display.get<TextComponent>()->text = buffer;
}

void BaseFighterScript::play_Animation(AnimationComponent *animation, uint8_t type)
{
    if (type != FIGHTER_ANIMATION_DEFAULT)
    {
        flags |= FIGHTER_PERFORMING_ACTION;
        input->set_Enabled_All(false);
    }
    m_Entity.set<AnimationComponent>(*animation);
}

void BaseFighterScript::end_Animation(void)
{
    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    if ((flags & FIGHTER_GROUND) == 0)
    {
        if (physics->velocity[1] < -300)
            a_Jump[0].frame_Offset = 1;
        else if (physics->velocity[1] < -100)
            a_Jump[0].frame_Offset = 2;
        else if (physics->velocity[1] < 100)
            a_Jump[0].frame_Offset = 3;
        else if (physics->velocity[1] < 300)
            a_Jump[0].frame_Offset = 4;
        else
            a_Jump[0].frame_Offset = 5;
        m_Entity.set<AnimationComponent>(a_Jump[0]);
    }
    else
    {
        if (axis_As_Int(input->x_Axis->get_Value()) == 0)
            m_Entity.set<AnimationComponent>(a_Idle[0]);
        else
        {
            m_Entity.set<AnimationComponent>(a_Run[0]);
        }
    }
}

void BaseFighterScript::set_Direction(int dir)
{
    if (dir)
    {
        auto transform = m_Entity.get<TransformComponent>();
        transform->scale[0] = dir;
        m_Entity.update_Transformation();
    }
}

bool BaseFighterScript::is_Phasing(void)
{
    return input->down->get_State() == State::PRESSED;
}