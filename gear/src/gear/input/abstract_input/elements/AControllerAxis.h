#pragma once

#include <gear/core/core.h>
#include "../AbstractAxis.h"
#include "../EventHandler.h"
#include "../../events/ControllerAxisEvent.h"

_GEAR_START

class AControllerAxis : public AbstractAxis, public EventHandler<ControllerAxisEvent>
{
private:
    int m_Controller_ID;
    ControllerAxis m_Axis;

public:
    AControllerAxis(int controller_ID, ControllerAxis axis);
    float get_Value(void) const override;
    void handle_Event(ControllerAxisEvent event) override;
};

_GEAR_END