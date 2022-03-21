#include "EisScript.h"

#include <gear/renderer/AnimationComponent.h>
#include <gear/renderer/SpriteComponent.h>

#include <gear/resource/ResourceManager.h>
#include <gear/resource/Sprite.h>
#include <gear/resource/Palette.h>

#include <gear/input/InputComponent.h>
#include <gear/input/events/KeyEvent.h>

#include <gear/core/debug/log.h>
#include <gear/scene/TransformComponent.h>

#include <gear/collision/DynamicPhysicsComponent.h>
#include <gear/collision/HitboxComponent.h>

#include <gear/collision/shapes/Circle.h>

#include <gear/scripting/ScriptComponent.h>

#include <gear/input/Input.h>

using namespace gear;

bool projectile_Physics_Check(CollisionEvent e)
{
    if (e.get_Other_Entity().get_Entity_ID() == 0)
        return false;
    return true;
}

static void spawn_Projectile(Entity e)
{
    Scene *scene = Scene::get(e.get_Scene_ID());
    Entity projectile = scene->create_Entity();
    auto transform = e.get<TransformComponent>();
    auto pos = transform->get_Matrix() * Vector<double, 3>{-17, 15, 1};
    projectile.add<TransformComponent>({pos.use_As<2>(), {1, 1}, transform->state});
    DynamicPhysicsComponent physics;
    physics.collider = Collider::create(Circle{{0, 0}, 5});
    physics.velocity = transform->state & GEAR_MIRROR_X ? 6 : -6;
    physics.acceleration = {0, 0};
    physics.restitution = 1;
    physics.check = projectile_Physics_Check;
    physics.on_Collision = [=](CollisionEvent e2)
    {
        if (e2.get_Other_Entity().get_Entity_ID() == 1)
            scene->remove_Entity(projectile);
    };
    projectile.add<DynamicPhysicsComponent>(physics);

    class ProjectileScript : gear::ScriptableEntity
    {
    private:
        int time_To_Live = 40;
        virtual void on_Update(void) override
        {
            time_To_Live--;
            if (time_To_Live <= 0)
                Scene::get(m_Entity.get_Scene_ID())->remove_Entity(m_Entity);
        }
    };

    projectile.add<ScriptComponent>(ScriptComponent().bind<ProjectileScript>());

    SpriteComponent sprite;
    sprite.sprite = ResourceManager::get<Sprite>("assets/test_sprites/triangle.gear");
    sprite.palette = ResourceManager::get<Palette>("assets/test_sprites/triangle_palette.gear");
    sprite.offset = {-5, -5, 0};
    projectile.add<SpriteComponent>(sprite);
};

EisScript::EisScript(int input) : m_Input(input)
{
}

void EisScript::on_Create(void)
{
    attack = m_Entity.get<HitboxComponent>()->hitboxes[0];
    InputComponent<KeyEvent> comp;
    comp.callback = [this](KeyEvent e)
    {
        if (e.get_Key() == Key::ENTER)
        {
            if (e.get_Action() == Action::PRESSED)
                spawn_Projectile(m_Entity);
        }
        if (e.get_Key() == Key::W && e.get_Action() == Action::PRESSED)
        {
            if (ground)
                m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -7;
            else if (jumps)
            {
                m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -5.5;
                jumps--;
            }
        }
        if (e.get_Key() == Key::SPACE && e.get_Action() == Action::PRESSED)
        {
            Vector<double, 2> flash_Dir = {0, 0};
            if (Input::get_Key_State(Key::A) == State::PRESSED)
                flash_Dir[0] -= 60;
            if (Input::get_Key_State(Key::D) == State::PRESSED)
                flash_Dir[0] += 60;
            if (Input::get_Key_State(Key::W) == State::PRESSED)
                flash_Dir[1] -= 60;
            if (Input::get_Key_State(Key::S) == State::PRESSED)
                flash_Dir[1] += 60;
            m_Entity.get<TransformComponent>()->position += flash_Dir;
            m_Entity.get<DynamicPhysicsComponent>()->velocity = 0;
        }
    };
    m_Entity.add<InputComponent<KeyEvent>>(comp);
    DynamicPhysicsComponent *physics = m_Entity.get<DynamicPhysicsComponent>();
    physics->on_Collision = [this](CollisionEvent e)
    {
        if (abs(e.get_Separation_Vector()[0]) < abs(e.get_Separation_Vector()[1]) / 20 && e.get_Separation_Vector()[1] > 0)
        {
            jumps = 3;
            ground = true;
        }
    };
}

void EisScript::on_Update(void)
{
    auto transform = m_Entity.get<TransformComponent>();
    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    ground = false;

    physics->velocity[0] = 0;

    auto axis_Value = Input::get_Axis_Value(0, ControllerAxis::LEFT_STICK_X);

    if (Input::get_Key_State(Key::A) == State::PRESSED)
    {
        physics->velocity[0] -= 3;
        transform->state = 0;
    }

    if (Input::get_Key_State(Key::D) == State::PRESSED)
    {
        physics->velocity[0] += 3;
        transform->state = GEAR_MIRROR_X;
    }

    m_Entity.update_Transformation();
}

void EisScript::on_Destroy(void)
{
}