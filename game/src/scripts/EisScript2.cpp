#include "EisScript2.h"

#include <gear/renderer/AnimationComponent.h>
#include <gear/input/InputComponent.h>
#include <gear/input/events/ControllerButtonEvent.h>

#include <gear/core/debug/log.h>
#include <gear/scene/TransformComponent.h>

#include <gear/collision/DynamicPhysicsComponent.h>

#include <gear/input/Input.h>

using namespace gear;

void EisScript2::on_Create(void)
{
    m_Entity.add<InputComponent<ControllerButtonEvent>>({[this](ControllerButtonEvent e)
                                                         {
                                                             if (e.get_Button() == ControllerButton::B && e.get_Action() == Action::PRESSED)
                                                             {
                                                                 if (ground)
                                                                     m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -7;
                                                                 else if (jumps)
                                                                 {
                                                                     m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -5.5;
                                                                     jumps--;
                                                                 }
                                                             }
                                                         }});
    DynamicPhysicsComponent *physics = m_Entity.get<DynamicPhysicsComponent>();
    physics->on_Collision = [this](CollisionEvent e)
    {
        if (abs(e.get_Separation_Vector()[0]) < abs(e.get_Separation_Vector()[1]) / 20 && e.get_Separation_Vector()[1] > 0)
        {
            jumps = 1;
            ground = true;
        }
    };
}

void EisScript2::on_Update(void)
{
    auto transform = m_Entity.get<TransformComponent>();
    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    ground = false;

    physics->velocity[0] = 0;

    auto axis_Value = Input::get_Axis_Value(0, ControllerAxis::LEFT_STICK_X);

    if (axis_Value < -0.4)
    {
        physics->velocity[0] -= 3;
        transform->state = 0;
    }

    if (axis_Value > 0.4)
    {
        physics->velocity[0] += 3;
        transform->state = GEAR_MIRROR_X;
    }

    m_Entity.update_Transformation();
}

void EisScript2::on_Destroy(void)
{
}