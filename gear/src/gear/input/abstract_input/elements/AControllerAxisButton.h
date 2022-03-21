#pragma once

#include <gear/core/core.h>
#include "../AbstractButton.h"
#include "../../EventQueue.h"
#include "../../events/ControllerAxisEvent.h"

_GEAR_START

class AControllerAxisButton : public AbstractButton, public EventHandler<ControllerAxisEvent>
{
private:
    int m_Controller_ID;
    ControllerAxis m_Button;
    float m_Intervall_Min;
    float m_Intervall_Max;
    State m_State = State::RELEASED;

public:
    AControllerAxisButton(int controller_ID, ControllerAxis button, float intervall_Min, float intervall_Max);

    State get_State(void) const override;
    void handle_Event(ControllerAxisEvent event) override;
    void set_Intervall(float intervall_Min, float intervall_Max);
    float get_Intervall_Min() const;
    float get_Intervall_Max() const;
};

_GEAR_END