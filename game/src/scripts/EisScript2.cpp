#include "EisScript2.h"

#include <gear/renderer/AnimationComponent.h>
#include <gear/event/EventComponent.h>
#include <gear/event/event_Types/KeyEvent.h>

#include <gear/core/debug/log.h>
#include <gear/scene/TransformComponent.h>

#include <gear/collision/PhysicsComponent.h>

#include <gear/event/Input.h>

using namespace gear;

void EisScript2::on_Create(void)
{
  m_Entity.add<EventComponent<ControllerButtonEvent>>({[&, this](ControllerButtonEvent e) {
    if(e.get_Button() == ControllerButton::B && e.get_Action() == Action::PRESSED) {
      this->m_Entity.get<PhysicsComponent>()->velocity[1] = -7;
    }
  }});
}

void EisScript2::on_Update(void)
{
  auto transform = m_Entity.get<TransformComponent>();
  auto physics = m_Entity.get<PhysicsComponent>();

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