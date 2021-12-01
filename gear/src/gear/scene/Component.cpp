#include "Component.h"

static unsigned int type_ID = 0;

unsigned int gear::get_Next_Component_ID(void) {
  if(type_ID >= GEAR_MAX_COMPONENTS - 1)
    gear::error("maximum amount %i of component type surpassed", GEAR_MAX_COMPONENTS);
  return type_ID++;
}