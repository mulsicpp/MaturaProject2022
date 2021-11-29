#include "Component.h"

static unsigned int type_ID = 0;

uint64_t gear::get_Next_Component_Flag(void) {
  if(type_ID >= GEAR_MAX_COMPONENTS - 1)
    gear::error("maximum amount of component type %i surpassed", GEAR_MAX_COMPONENTS);
  return GEAR_BIT_TYPE(uint64_t, type_ID++);
}