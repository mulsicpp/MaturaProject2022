#pragma once

#include <gear/core/core.h>
#include "../events/Event.h"

_GEAR_START

enum class ControllerStatus
{
    CONNECTED = GLFW_CONNECTED,
    DISCONNECTED = GLFW_DISCONNECTED
};

class ControllerConnectionEvent : public Event
{
    friend class Game;

private:
    static void controller_Connection_Event_Callback(int jid, int event);
    int m_Controller_Id;
    ControllerStatus m_status;
    ControllerConnectionEvent(int controller_ID, int controller_Status);

public:
    ControllerStatus get_Controller_Status(void) const;
    int get_Controller_Id(void) const;
};
_GEAR_END