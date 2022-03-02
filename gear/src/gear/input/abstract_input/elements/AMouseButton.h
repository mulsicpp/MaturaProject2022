#pragma once

#include <gear/core/core.h>
#include "../AbstractButton.h"
#include "../EventHandler.h"
#include "../../events/MouseButtonEvent.h"

_GEAR_START

class AMouseButton : public AbstractButton, public EventHandler<MouseButtonEvent> {
private:
  MouseButton m_Button;

public:
  AMouseButton(MouseButton button);

  State get_State(void) const override;
  void handle_Event(MouseButtonEvent event) override;
};

_GEAR_END