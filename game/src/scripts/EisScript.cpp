#include "EisScript.h"

#include <gear/renderer/AnimationComponent.h>
#include <gear/event/EventComponent.h>
#include <gear/event/event_Types/KeyEvent.h>

#include <gear/core/debug/log.h>

void EisScript::on_Create(void)
{
  GEAR_DEBUG_LOG("on create %i", m_Entity.get_Entity_ID());
}

void EisScript::on_Update(void)
{
  GEAR_DEBUG_LOG("on update %i", m_Entity.get_Entity_ID());
}

void EisScript::on_Destroy(void)
{
  GEAR_DEBUG_LOG("on destroy %i", m_Entity.get_Entity_ID());
}