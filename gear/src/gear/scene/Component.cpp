#include "Component.h"
#include <gear/renderer/SpriteComponent.h>
#include <gear/renderer/AnimationComponent.h>
#include <gear/renderer/TextComponent.h>

#include <gear/event/EventComponent.h>
#include <gear/event/event_Types/KeyEvent.h>
#include <gear/event/event_Types/TextEvent.h>
#include <gear/event/event_Types/MouseButtonEvent.h>
#include <gear/event/event_Types/MouseMovedEvent.h>
#include <gear/event/event_Types/ScrollEvent.h>
#include <gear/event/event_Types/WindowFocusEvent.h>
#include <gear/event/event_Types/WindowIconifyEvent.h>
#include <gear/event/event_Types/ControllerConnectionEvent.h>
#include <gear/event/event_Types/ControllerAxisEvent.h>
#include <gear/event/event_Types/ControllerButtonEvent.h>

#include <gear/scripting/ScriptComponent.h>

#include "TransformComponent.h"

static unsigned int type_ID = 0;

void gear::allow_Gear_Components(void) {
  Component<TransformComponent>::allow();

  Component<ScriptComponent>::allow();

  Component<SpriteComponent>::allow();
  Component<AnimationComponent>::allow();
  Component<TextComponent>::allow();

  Component<EventComponent<KeyEvent>>::allow();
  Component<EventComponent<TextEvent>>::allow();

  Component<EventComponent<MouseButtonEvent>>::allow();
  Component<EventComponent<MouseMovedEvent>>::allow();
  Component<EventComponent<ScrollEvent>>::allow();

  Component<EventComponent<WindowFocusEvent>>::allow();
  Component<EventComponent<WindowIconifyEvent>>::allow();

  Component<EventComponent<ControllerConnectionEvent>>::allow();
  Component<EventComponent<ControllerAxisEvent>>::allow();
  Component<EventComponent<ControllerButtonEvent>>::allow();
}

unsigned int gear::get_Next_Component_ID(void) {
  if(type_ID >= GEAR_MAX_COMPONENTS - 1)
    gear::error("maximum amount %i of component type surpassed", GEAR_MAX_COMPONENTS);
  return type_ID++;
}