#pragma once

#include <gear/input/abstract_input/abstract_input.h>
#include <gear/input/abstract_input/AbstractButton.h>

GEAR_ABSTRACT_INPUT(FighterInput, KFighterInput, CFighterInput, GEAR_BUTTONS(left, right, up, down, jump, attack, special, shield));