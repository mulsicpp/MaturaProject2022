#include "BaseFighterScript.h"

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
