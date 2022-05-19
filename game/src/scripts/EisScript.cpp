#include "EisScript.h"

#include <gear/collision/shapes/Rect.h>
#include <gear/collision/shapes/Circle.h>

#include <gear/resource/ResourceManager.h>

#include "../components/FlagComponent.h"

#include <gear/core/Game.h>

#include <gear/scripting/ScriptComponent.h>

using namespace gear;

EisScript::EisScript(InputDevice device, std::string palette_Name) : BaseFighterScript(device, "assets/fighters/eis", palette_Name) {
    physics.collider = Collider::create(Rect{{-12, 10}, {12, 28}});
}

void EisScript::init_Animation_Events(void) {
    a_Projectile->animation = ResourceManager::get<Animation>("assets/fighters/eis/animations/right/projectile.gear");
    a_Projectile->palette = palette;
    a_Projectile->offset = {-8, -8, 0.05};
    a_Projectile->frame_Offset = 0;
    a_Projectile->frame_Rate = a_Projectile->animation->get_Frame_Rate();

    a_Sspecial->on_Frame_Changed = [this](int frame) {
        if(frame == 5) {
            spawn_Projectile();
        }
    };
}

void EisScript::spawn_Projectile(void) {
    Scene *current_Scene = Scene::get(m_Entity.get_Scene_ID());
    Entity proj = current_Scene->create_Entity();

    proj.set<FlagComponent>({FLAG_PROJECTILE});

    auto transform = *m_Entity.get<TransformComponent>();
    transform.position[0] += 26 * transform.scale[0];
    transform.position[1] += 10;
    proj.set<TransformComponent>(transform);

    proj.add<AnimationComponent>(a_Projectile[0]);

    DynamicPhysicsComponent physics;
    physics.acceleration = {0, 0};
    physics.collider = Collider::create(Circle{{0, 0}, 7});
    physics.velocity = 100 * transform.scale[0];
    proj.add<DynamicPhysicsComponent>(physics);
    proj.update_Transformation();

    class ProjectileScript : public ScriptableEntity {
    private:
        double time_To_Live = 1;

        void pre_Input(void) override {
            time_To_Live -= Game::get_Delta_Time();
            if(time_To_Live <= 0)
                Scene::get(m_Entity.get_Scene_ID())->remove_Entity(m_Entity);
        }
    };

    proj.add<ScriptComponent>(ScriptComponent().bind<ProjectileScript>());

    GEAR_DEBUG_LOG("successfully created projectile");
}