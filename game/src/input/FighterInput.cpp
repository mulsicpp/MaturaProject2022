#include "FighterInput.h"

#include <gear/input/abstract_input/elements/AKeyboardButton.h>
#include <gear/input/abstract_input/elements/AMouseButton.h>

#include <gear/input/abstract_input/elements/AControllerAxisButton.h>
#include <gear/input/abstract_input/elements/AControllerButton.h>

using namespace gear;

KFighterInput::KFighterInput(void)
{
    left = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::A), GEAR_CREATE_BUTTON(AKeyboardButton, Key::LEFT));
    right = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::D), GEAR_CREATE_BUTTON(AKeyboardButton, Key::RIGHT));
    up = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::W), GEAR_CREATE_BUTTON(AKeyboardButton, Key::UP));
    down = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::S), GEAR_CREATE_BUTTON(AKeyboardButton, Key::DOWN));
    
    jump = GEAR_CREATE_OR_BUTTON(up, GEAR_CREATE_BUTTON(AKeyboardButton, Key::SPACE));

    attack = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::NP_SUBTRACT), GEAR_CREATE_BUTTON(AMouseButton, MouseButton::LEFT));
    special = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::NP_ADD), GEAR_CREATE_BUTTON(AMouseButton, MouseButton::RIGHT));

    shield = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::LEFT_SHIFT), GEAR_CREATE_BUTTON(AKeyboardButton, Key::RIGHT_SHIFT));
}

CFighterInput::CFighterInput(int id) : AbstractControllerInput(id)
{
    left = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_X, -1, -0.4);
    right = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_X, 0.4, 1);
    up = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, -1, -0.4);
    down = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, 0.4, 1);

    jump = GEAR_CREATE_OR_BUTTON(up, GEAR_CREATE_BUTTON(AKeyboardButton, Key::SPACE));

    attack = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::B);
    special = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::A);

    shield = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::LEFT_BUMPER), GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::RIGHT_BUMPER));
}