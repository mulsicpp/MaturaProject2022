#include "EisScript.h"

#include <gear/collision/shapes/Rect.h>

using namespace gear;

EisScript::EisScript(InputDevice device) : BaseFighterScript(device, "assets/fighters/medusa") {
    physics.collider = Collider::create(Rect{{-12, 14}, {12, 32}});
}