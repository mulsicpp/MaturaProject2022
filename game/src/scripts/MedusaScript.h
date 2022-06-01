#pragma once

#include "BaseFighterScript.h"

class MedusaScript : public BaseFighterScript
{
public:
    MedusaScript(gear::InputDevice device, std::string palette_Name, int player_Number);

    virtual void init(void) override;
    virtual void init_Animation_Events(void) override;
};