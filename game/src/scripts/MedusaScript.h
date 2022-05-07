#pragma once

#include "BaseFighterScript.h"

class MedusaScript : public BaseFighterScript
{
public:
    MedusaScript(gear::InputDevice device);
    virtual void init_Animation_Events(void) override;
};