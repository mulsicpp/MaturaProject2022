#include "ScriptComponent.h"
#include <gear/core/debug/log.h>
#include <functional>
#include <vector>

static std::vector<std::function<void(void)>> callbacks(0);

static void(gear::ScriptableEntity::*function)(void) = &gear::ScriptableEntity::post_Hitbox_Check;

static void script_Callback(gear::ScriptComponent &script)
{
    callbacks.push_back([&]() {(script.script->*function)(); });
}

void gear::call_Script_Function(void(gear::ScriptableEntity::*member)(void), gear::Scene *scene)
{
    function = member;
    gear::Entity::for_Each(scene->get_ID(), script_Callback);
    for(auto &callback : callbacks) {
        callback();
    }
    callbacks.clear();
}