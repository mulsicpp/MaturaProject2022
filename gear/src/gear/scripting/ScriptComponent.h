#pragma once

#include <gear/core/core.h>
#include "ScriptableEntity.h"
#include <gear/scene/Component.h>
#include <gear/core/debug/log.h>
#include <functional>

_GEAR_START

struct ScriptComponent
{
private:
  template <class T, class... Args>
  static ScriptableEntity *construct(Args ...args)
  {
    return (ScriptableEntity*)(new T(args...));
  }

  template <class T>
  static void destruct(ScriptComponent *comp)
  {
    delete comp->script;
    comp->script = nullptr;
  }

public:
  ScriptableEntity *script;

  std::function<ScriptableEntity *(void)> construct_Script;
  std::function<void (ScriptComponent *)>destruct_Script;

  template <class T, class... Args>
  ScriptComponent& bind(Args... args)
  {
    construct_Script = std::bind(construct<T, Args...>, args...);
    destruct_Script = destruct<T>;
    return *this;
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