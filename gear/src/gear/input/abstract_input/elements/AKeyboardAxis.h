#pragma once

#include <gear/core/core.h>
#include "../AbstractAxis.h"
#include "../EventHandler.h"
#include "../../events/KeyEvent.h"

_GEAR_START

class AKeyboardAxis : public AbstractAxis, public EventHandler<KeyEvent>
{
private:
    Key m_Min_Key;
    Key m_Max_Key;

public:
    AKeyboardAxis(Key min_Key, Key max_Key);
    float get_Value(void) const override;
    void handle_Event(KeyEvent event) override;
};

_GEAR_END