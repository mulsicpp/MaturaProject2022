#include "Component.h"

static uint64_t type_ID = 0;

uint64_t gear::get_Next_Component_Flag(void) {
  return GEAR_BIT_TYPE(uint64_t, type_ID++);
}