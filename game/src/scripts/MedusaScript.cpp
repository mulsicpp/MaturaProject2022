#include "MedusaScript.h"

#include <gear/collision/shapes/Rect.h>

#include <gear/collision/HurtboxComponent.h>

using namespace gear;

MedusaScript::MedusaScript(InputDevice device, std::string palette_Name, int player_Number) : BaseFighterScript(device, "assets/fighters/medusa", palette_Name, player_Number) {
    physics.collider = Collider::create(Rect{{-6, -28}, {6, 25}});

    max_Health = 370;
    health = 370;
}

void MedusaScript::init(void)
{
    HurtboxComponent comp;
    comp.hurtboxes = {Hurtbox::create(0, Rect{{-6, -28}, {6, 25}})};

    m_Entity.add(comp);

    BaseFighterScript::init();
}

void MedusaScript::init_Animation_Events(void) {

}