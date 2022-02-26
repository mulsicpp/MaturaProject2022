#include "EisScript2.h"

#include <gear/renderer/AnimationComponent.h>
#include <gear/event/EventComponent.h>
#include <gear/event/event_Types/KeyEvent.h>

#include <gear/core/debug/log.h>
#include <gear/scene/TransformComponent.h>

#include <gear/event/Input.h>

using namespace gear;

void EisScript2::on_Create(void)
{

}

void EisScript2::on_Update(void)
{
  auto transform = m_Entity.get<TransformComponent>();

  if (Input::get_Key_State(Key::LEFT) == State::PRESSED)
  {
    transform->position[0] -= 3;
    transform->state = 0;
  }

  if (Input::get_Key_State(Key::RIGHT) == State::PRESSED)
  {
    transform->position[0] += 3;
    transform->state = GEAR_MIRROR_X;
  }

  if (Input::get_Key_State(Key::UP) == State::PRESSED)
    transform->position[1] -= 3;

  if (Input::get_Key_State(Key::DOWN) == State::PRESSED)
    transform->position[1] += 3;
    
  m_Entity.update_Transformation();
}

void EisScript2::on_Destroy(void)
{
}