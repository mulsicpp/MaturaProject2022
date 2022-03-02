#pragma once

#include <gear/core/core.h>
#include "../AbstractButton.h"
#include "../EventHandler.h"
#include "../../events/ControllerButtonEvent.h"

_GEAR_START

class AControllerButton : public AbstractButton, public EventHandler<ControllerButtonEvent> {
private:
  int m_Controller_ID;
  ControllerButton m_Button;

public:
  AControllerButton(int controller_ID, ControllerButton button);

  State get_State(void) const override;
  void handle_Event(ControllerButtonEvent event) override;
};

_GEAR_END