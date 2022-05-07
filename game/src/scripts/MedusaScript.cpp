#include "MedusaScript.h"

#include <gear/collision/shapes/Rect.h>

using namespace gear;

MedusaScript::MedusaScript(InputDevice device) : BaseFighterScript(device, "assets/fighters/medusa") {
    physics.collider = Collider::create(Rect{{-6, -28}, {6, 25}});
}

void MedusaScript::init_Animation_Events(void) {

}