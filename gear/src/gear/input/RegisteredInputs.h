#pragma once

#include <gear/core/core.h>
#include <vector>
#include "InputDevice.h"

_GEAR_START

class RegisteredInputs
{
private:
    static std::vector<InputDevice> m_Registered_Inputs;

public:
    static std::vector<InputDevice> get_All(void);
    static InputDevice get(int index);
    static void add(InputDevice input);
    static void remove(InputDevice input);
    static void remove(int index);
    static void remove_All(void);
};

_GEAR_END