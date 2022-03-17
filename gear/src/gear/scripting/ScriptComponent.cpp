#include "ScriptComponent.h"
#include <gear/core/debug/log.h>
#include <functional>
#include <vector>

static std::vector<std::function<void(void)>> callbacks(0);

static void script_Callback(gear::ScriptComponent &script)
{
    callbacks.push_back([&]() {script.script->on_Update(); });
}

void gear::call_Script_Update(gear::Scene *scene)
{
    gear::Entity::for_Each(scene->get_ID(), script_Callback);
    for(auto &callback : callbacks) {
        callback();
    }
    callbacks.clear();
}