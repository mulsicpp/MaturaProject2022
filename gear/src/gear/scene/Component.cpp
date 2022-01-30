#include "Component.h"
#include <gear/renderer/SpriteComponent.h>
#include <gear/renderer/AnimationComponent.h>
#include <gear/renderer/TextComponent.h>
#include "TransformComponent.h"

static unsigned int type_ID = 0;

void gear::allow_Gear_Components(void) {
  gear::Component<gear::TransformComponent>::allow();
  gear::Component<gear::SpriteComponent>::allow();
  gear::Component<gear::AnimationComponent>::allow();
  gear::Component<gear::TextComponent>::allow();
}

unsigned int gear::get_Next_Component_ID(void) {
  if(type_ID >= GEAR_MAX_COMPONENTS - 1)
    gear::error("maximum amount %i of component type surpassed", GEAR_MAX_COMPONENTS);
  return type_ID++;
}