#include "MedusaScript.h"

#include <gear/collision/shapes/Rect.h>

using namespace gear;

MedusaScript::MedusaScript(InputDevice device, std::string palette_Name) : BaseFighterScript(device, "assets/fighters/medusa", palette_Name) {
    physics.collider = Collider::create(Rect{{-6, -28}, {6, 25}});
}

void MedusaScript::init_Animation_Events(void) {

}