#pragma once

#include <gear/core/core.h>
#include "ScriptableEntity.h"
#include <gear/scene/Component.h>
#include <gear/core/debug/log.h>

_GEAR_START

struct ScriptComponent
{
private:
  template <class T>
  static ScriptableEntity *construct(void)
  {
    return (ScriptableEntity*)(new T());
  }

  template <class T>
  static void destruct(ScriptComponent *comp)
  {
    delete comp->script;
    comp->script = nullptr;
  }

public:
  ScriptableEntity *script;

  ScriptableEntity *(*construct_Script)(void);
  void (*destruct_Script)(ScriptComponent *);

  template <class T>
  void bind(void)
  {
    construct_Script = construct<T>;
    destruct_Script = destruct<T>;
  }

};

template <>
void Component<ScriptComponent>::on_Component_Add(Entity entity, ScriptComponent *component)
{
  component->script = component->construct_Script();
  component->script->m_Entity = entity;
  GEAR_DEBUG_LOG("constructed script %p", component->script);
  component->script->on_Create();
}

template <>
void Component<ScriptComponent>::on_Component_Remove(Entity entity, ScriptComponent *component)
{
  component->script->on_Destroy();
  component->destruct_Script(component);
  GEAR_DEBUG_LOG("destructed script %p", component->script);
}

void call_Script_Update(Scene *scene);

_GEAR_END