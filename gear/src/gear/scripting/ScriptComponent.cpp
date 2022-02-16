#include "ScriptComponent.h"

static void script_Callback(gear::ScriptComponent &script) 
{
  script.script->on_Update();
}

void gear::call_Script_Update(gear::Scene *scene) {
  gear::Entity::for_Each(scene->get_ID(), script_Callback);
}