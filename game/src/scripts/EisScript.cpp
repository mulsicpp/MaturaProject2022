#include "EisScript.h"

#include <gear/collision/shapes/Rect.h>

using namespace gear;

EisScript::EisScript(InputDevice device) : BaseFighterScript(device, "assets/fighters/eis") {
    physics.collider = Collider::create(Rect{{-12, 10}, {12, 28}});
}