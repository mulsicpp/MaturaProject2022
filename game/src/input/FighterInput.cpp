#include "FighterInput.h"

#include <gear/input/abstract_input/elements/AKeyboardButton.h>
#include <gear/input/abstract_input/elements/AMouseButton.h>
#include <gear/input/abstract_input/elements/AControllerAxisButton.h>
#include <gear/input/abstract_input/elements/AControllerButton.h>

#include <gear/input/abstract_input/elements/AKeyboardAxis.h>
#include <gear/input/abstract_input/elements/AControllerAxis.h>

using namespace gear;

void FighterInput::set_Enabled_All(bool enabled)
{
    x_Axis->set_Enabled(enabled);

    up->set_Enabled(enabled);
    down->set_Enabled(enabled);

    attack->set_Enabled(enabled);
    special->set_Enabled(enabled);

    jump->set_Enabled(enabled);

    shield->set_Enabled(enabled);
}

KFighterInput::KFighterInput(void)
{
    x_Axis = GEAR_CREATE_OR_AXIS(GEAR_CREATE_AXIS(AKeyboardAxis, Key::A, Key::D), GEAR_CREATE_AXIS(AKeyboardAxis, Key::LEFT, Key::RIGHT));

    up = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::W), GEAR_CREATE_BUTTON(AKeyboardButton, Key::UP));
    down = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::S), GEAR_CREATE_BUTTON(AKeyboardButton, Key::DOWN));

    jump = GEAR_CREATE_BUTTON(AKeyboardButton, Key::SPACE);

    attack = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::I), GEAR_CREATE_BUTTON(AMouseButton, MouseButton::LEFT));
    special = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::O), GEAR_CREATE_BUTTON(AMouseButton, MouseButton::RIGHT));

    shield = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::LEFT_SHIFT), GEAR_CREATE_BUTTON(AKeyboardButton, Key::RIGHT_SHIFT));
}

CFighterInput::CFighterInput(int id) : AbstractControllerInput(id)
{
    x_Axis = GEAR_CREATE_AXIS(AControllerAxis, id, ControllerAxis::LEFT_STICK_X);

    up = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, -1, -0.4);
    down = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, 0.4, 1);

    jump = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::X);

    attack = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::B);
    special = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::A);

    shield = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::LEFT_BUMPER), GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::RIGHT_BUMPER));
}