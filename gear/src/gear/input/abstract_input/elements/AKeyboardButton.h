#pragma once

#include <gear/core/core.h>
#include "../AbstractButton.h"
#include "../../EventQueue.h"
#include "../../events/KeyEvent.h"

_GEAR_START

class AKeyboardButton : public AbstractButton, public EventHandler<KeyEvent> {
private:
  Key m_Button;

public:
  AKeyboardButton(Key button);

  State get_State(void) const override;
  void handle_Event(KeyEvent event) override;
};

_GEAR_END