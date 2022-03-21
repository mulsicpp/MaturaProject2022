#include "Component.h"
#include <gear/renderer/SpriteComponent.h>
#include <gear/renderer/AnimationComponent.h>
#include <gear/renderer/TextComponent.h>

#include <gear/input/InputComponent.h>
#include <gear/input/events/KeyEvent.h>
#include <gear/input/events/TextEvent.h>
#include <gear/input/events/MouseButtonEvent.h>
#include <gear/input/events/MouseMovedEvent.h>
#include <gear/input/events/ScrollEvent.h>
#include <gear/input/events/WindowFocusEvent.h>
#include <gear/input/events/WindowIconifyEvent.h>
#include <gear/input/events/ControllerConnectionEvent.h>
#include <gear/input/events/ControllerAxisEvent.h>
#include <gear/input/events/ControllerButtonEvent.h>

#include <gear/collision/DynamicPhysicsComponent.h>
#include <gear/collision/HitboxComponent.h>
#include <gear/collision/HurtboxComponent.h>

#include <gear/scripting/ScriptComponent.h>

#include "TransformComponent.h"

static unsigned int type_ID = 0;

void gear::allow_Gear_Components(void) {
  Component<TransformComponent>::allow();

  Component<ScriptComponent>::allow();

  Component<SpriteComponent>::allow();
  Component<AnimationComponent>::allow();
  Component<TextComponent>::allow();

  Component<InputComponent<KeyEvent>>::allow();
  Component<InputComponent<TextEvent>>::allow();

  Component<InputComponent<MouseButtonEvent>>::allow();
  Component<InputComponent<MouseMovedEvent>>::allow();
  Component<InputComponent<ScrollEvent>>::allow();

  Component<InputComponent<WindowFocusEvent>>::allow();
  Component<InputComponent<WindowIconifyEvent>>::allow();

  Component<InputComponent<ControllerConnectionEvent>>::allow();
  Component<InputComponent<ControllerAxisEvent>>::allow();
  Component<InputComponent<ControllerButtonEvent>>::allow();

  Component<StaticPhysicsComponent>::allow();
  Component<DynamicPhysicsComponent>::allow();

  Component<HitboxComponent>::allow();
  Component<HurtboxComponent>::allow();
}

unsigned int gear::get_Next_Component_ID(void) {
  if(type_ID >= GEAR_MAX_COMPONENTS - 1)
    gear::error("maximum amount %i of component type surpassed", GEAR_MAX_COMPONENTS);
  return type_ID++;
}