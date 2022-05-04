#pragma once

#include <gear/input/abstract_input/abstract_input.h>
#include <gear/input/abstract_input/AbstractButton.h>
#include <gear/input/abstract_input/AbstractAxis.h>

GEAR_ABSTRACT_INPUT(FighterInput, KFighterInput, CFighterInput, 
GEAR_BUTTONS(up, down, jump, attack, special, shield)
GEAR_AXES(x_Axis)
static void set_Enabled_All(gear::Ref<FighterInput> input, bool enabled);
);