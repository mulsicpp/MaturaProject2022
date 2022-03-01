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
  m_Entity.add<EventComponent<KeyEvent>>({[&, this](KeyEvent e) {
    if(e.get_Key() == Key::UP && e.get_Action() == Action::PRESSED) {
      this->m_Entity.get<PhysicsComponent>()->velocity[1] = -7;
    }
  }});
}

void EisScript2::on_Update(void)
{
  auto transform = m_Entity.get<TransformComponent>();
  auto physics = m_Entity.get<PhysicsComponent>();

  physics->velocity[0] = 0;

  if (Input::get_Key_State(Key::LEFT) == State::PRESSED)
  {
    physics->velocity[0] -= 3;
    transform->state = 0;
  }

  if (Input::get_Key_State(Key::RIGHT) == State::PRESSED)
  {
    physics->velocity[0] += 3;
    transform->state = GEAR_MIRROR_X;
  }
    
  m_Entity.update_Transformation();
}

void EisScript2::on_Destroy(void)
{
}