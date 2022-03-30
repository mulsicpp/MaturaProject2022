#include "BaseFighterScript.h"

#include <gear/resource/ResourceManager.h>

using namespace gear;

BaseFighterScript::BaseFighterScript(InputDevice device, const char *base_Path)
{
    input = FighterInput::create_From(device);

    init_Input();
    init_Animations(base_Path);
}

BaseFighterScript::~BaseFighterScript() {}

void BaseFighterScript::init(void) {

}

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

void BaseFighterScript::pre_Input(void) {

}