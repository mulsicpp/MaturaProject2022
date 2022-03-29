#include "BaseFighterScript.h"

#include <gear/resource/ResourceManager.h>

using namespace gear;

BaseFighterScript::BaseFighterScript(InputDevice device)
{
    input = FighterInput::create_From(device);

    init_Input();
}

BaseFighterScript::~BaseFighterScript() {}

void BaseFighterScript::init_Input(void)
{
    input->attack->set_Callback(
        [this](Action a)
        {
            if (a == Action::PRESSED)
            {
                if (input->left->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("sground");
                else if (input->right->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("sground");
                else if (input->up->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("uground");
                else if (input->down->get_State() == State::PRESSED)
                    GEAR_DEBUG_LOG("dground");
                else
                    GEAR_DEBUG_LOG("nground");
            }
        });
}

static void init_Animation(AnimationComponent *animation, std::string path, Ref<Palette> palette, AnimationType type) {

    animation->animation = ResourceManager::get<Animation>(path);
    animation->palette = palette;
    animation->type = type;

    animation->offset = {-40, -40, 0};
    animation->parallax_Factor = 1;
    animation->frame_Offset = 0;
    animation->frame_Rate = animation->animation->get_Default_Frame_Rate();
}

void BaseFighterScript::init_Animations(const char *base_Path, const char *palette_Path)
{
    Ref<Palette> palette = ResourceManager::get<Palette>(palette_Path);
    std::string path = base_Path;

    init_Animation(&a_Idle, path + "/idle.gear", palette, AnimationType::LOOP);
    init_Animation(&a_Run, path + "/run.gear", palette, AnimationType::LOOP);
    init_Animation(&a_Jump, path + "/jump.gear", palette, AnimationType::FORWARD);

    init_Animation(&a_Sground, path + "/sground.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Uground, path + "/uground.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Dground, path + "/dground.gear", palette, AnimationType::FORWARD);

    init_Animation(&a_Sair, path + "/sair.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Uair, path + "/uair.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Dair, path + "/dair.gear", palette, AnimationType::FORWARD);

    init_Animation(&a_Sspecial, path + "/sspecial.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Uspecial, path + "/uspecial.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Dspecial, path + "/dspecial.gear", palette, AnimationType::FORWARD);

    init_Animation(&a_Ult, path + "/ult.gear", palette, AnimationType::FORWARD);

    init_Animation(&a_Damaged, path + "/damaged.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Shield, path + "/shield.gear", palette, AnimationType::LOOP);

    init_Animation(&a_Ledge_Grab, path + "/ledge_grab.gear", palette, AnimationType::FORWARD);
    init_Animation(&a_Hanging, path + "/hanging.gear", palette, AnimationType::LOOP);
    init_Animation(&a_Get_Up, path + "/get_up.gear", palette, AnimationType::FORWARD);
}