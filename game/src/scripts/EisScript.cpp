#include "EisScript.h"

#include <gear/renderer/AnimationComponent.h>
#include <gear/event/EventComponent.h>
#include <gear/event/event_Types/KeyEvent.h>

#include <gear/core/debug/log.h>
#include <gear/scene/TransformComponent.h>

#include <gear/collision/DynamicPhysicsComponent.h>

#include <gear/event/Input.h>

using namespace gear;

void EisScript::on_Create(void)
{
    m_Entity.add<EventComponent<KeyEvent>>({[this](KeyEvent e)
                                            {
                                                if (e.get_Key() == Key::W && e.get_Action() == Action::PRESSED)
                                                {
                                                    if (ground)
                                                        m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -7;
                                                    else if (jumps)
                                                    {
                                                        m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -7;
                                                        jumps--;
                                                    }
                                                }
                                                if (e.get_Key() == Key::SPACE && e.get_Action() == Action::PRESSED)
                                                {
                                                    Vector<double, 2> flash_Dir = {0, 0};
                                                    if (Input::get_Key_State(Key::A) == State::PRESSED)
                                                        flash_Dir[0] -= 60;
                                                    if (Input::get_Key_State(Key::D) == State::PRESSED)
                                                        flash_Dir[0] += 60;
                                                    if (Input::get_Key_State(Key::W) == State::PRESSED)
                                                        flash_Dir[1] -= 60;
                                                    if (Input::get_Key_State(Key::S) == State::PRESSED)
                                                        flash_Dir[1] += 60;
                                                    m_Entity.get<TransformComponent>()->position += flash_Dir;
                                                    m_Entity.get<DynamicPhysicsComponent>()->velocity = 0;
                                                }
                                            }});
    DynamicPhysicsComponent *physics = m_Entity.get<DynamicPhysicsComponent>();
    physics->on_Collision = [this](CollisionEvent e)
    {
        GEAR_DEBUG_LOG("collision occured");
        if (abs(e.get_Separation_Vector()[0]) < abs(e.get_Separation_Vector()[1]) / 20 && e.get_Separation_Vector()[1] > 0)
        {
            jumps = 1;
            ground = true;
        }
    };
}

void EisScript::on_Update(void)
{
    auto transform = m_Entity.get<TransformComponent>();
    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    ground = false;

    physics->velocity[0] = 0;

    auto axis_Value = Input::get_Axis_Value(0, ControllerAxis::LEFT_STICK_X);

    if (Input::get_Key_State(Key::A) == State::PRESSED)
    {
        physics->velocity[0] -= 3;
        transform->state = 0;
    }

    if (Input::get_Key_State(Key::D) == State::PRESSED)
    {
        physics->velocity[0] += 3;
        transform->state = GEAR_MIRROR_X;
    }

    m_Entity.update_Transformation();
}

void EisScript::on_Destroy(void)
{
}