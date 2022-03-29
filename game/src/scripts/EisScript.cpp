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

#include <gear/input/abstract_input/elements/AKeyboardButton.h>
#include <gear/input/abstract_input/elements/AMouseButton.h>
#include <gear/input/abstract_input/elements/AControllerAxisButton.h>
#include <gear/input/abstract_input/elements/AControllerButton.h>

#include <gear/input/abstract_input/elements/AKeyboardAxis.h>
#include <gear/input/abstract_input/elements/AControllerAxis.h>

#include <gear/input/Input.h>

#include <gear/core/Game.h>

using namespace gear;

KEisInput::KEisInput(void)
{
    left = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::A), GEAR_CREATE_BUTTON(AKeyboardButton, Key::LEFT));
    right = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::D), GEAR_CREATE_BUTTON(AKeyboardButton, Key::RIGHT));
    up = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::W), GEAR_CREATE_BUTTON(AKeyboardButton, Key::UP));
    down = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::S), GEAR_CREATE_BUTTON(AKeyboardButton, Key::DOWN));
    attack = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::ENTER), GEAR_CREATE_BUTTON(AMouseButton, MouseButton::LEFT));
    flash = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AKeyboardButton, Key::SPACE), GEAR_CREATE_BUTTON(AKeyboardButton, Key::F));

    x_Axis = GEAR_CREATE_OR_AXIS(GEAR_CREATE_AXIS(AKeyboardAxis, Key::A, Key::D), GEAR_CREATE_AXIS(AKeyboardAxis, Key::LEFT, Key::RIGHT));
}

CEisInput::CEisInput(int id) : AbstractControllerInput(id)
{
    left = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_X, -1, -0.4);
    right = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_X, 0.4, 1);
    up = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, -1, -0.4);
    down = GEAR_CREATE_BUTTON(AControllerAxisButton, id, ControllerAxis::LEFT_STICK_Y, 0.4, 1);
    attack = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::B);
    flash = GEAR_CREATE_BUTTON(AControllerButton, id, ControllerButton::Y);

    x_Axis = GEAR_CREATE_AXIS(AControllerAxis, id, ControllerAxis::LEFT_STICK_X);
}

bool projectile_Physics_Check(CollisionEvent e)
{
    if (e.get_Other_Entity().get_Entity_ID() == 0 || e.get_Other_Entity().get_Entity_ID() == 1)
        return false;
    return true;
}

void projectile_Physics_On_Collision(CollisionEvent e)
{
    Scene::get(e.get_Other_Entity().get_Scene_ID())->remove_Entity(e.get_Entity());
}

static void spawn_Projectile(Entity e)
{
    Scene *scene = Scene::get(e.get_Scene_ID());
    Entity projectile = scene->create_Entity();
    auto transform = e.get<TransformComponent>();
    auto pos = transform->get_Matrix() * Vector<double, 3>{-15, 20, 1};
    projectile.add<TransformComponent>({pos.use_As<2>(), {1, 1}, transform->state});

    DynamicPhysicsComponent physics;
    physics.collider = Collider::create(Circle{{0, 0}, 5});
    physics.velocity = transform->state & GEAR_MIRROR_X ? 720 : -720;
    physics.acceleration = {0, 0};
    physics.restitution = 1;
    physics.check = projectile_Physics_Check;
    physics.on_Collision = projectile_Physics_On_Collision;
    projectile.add<DynamicPhysicsComponent>(physics);

    HitboxComponent hitbox;
    hitbox.hitboxes = {Hitbox::create(1, Circle{{0, 0}, 5})};
    hitbox.hitboxes[0]->on_Collision_Begin(
        [](HitboxEvent e) {
            Entity entity = e.get_Entity();
            Entity other_Entity = e.get_Other_Entity();
            other_Entity.get<TransformComponent>()->position[0] += entity.get<DynamicPhysicsComponent>()->velocity[0] * 0.01;
            Scene::get(entity.get_Scene_ID())->remove_Entity(entity);
            other_Entity.update_Transformation();
        }
    );
    projectile.add(hitbox);

    class ProjectileScript : gear::ScriptableEntity
    {
    private:
        double time_To_Live = 1;
        virtual void post_Input(void) override
        {
            time_To_Live -= gear::Game::get_Delta_Time();
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

EisScript::EisScript(InputDevice device)
{
    m_Input = EisInput::create_From(device);
}

void EisScript::init(void)
{
    attack = m_Entity.get<HitboxComponent>()->hitboxes[0];

    m_Input->attack->set_Callback(
        [this](Action a)
        {
            if (a == Action::PRESSED)
                spawn_Projectile(m_Entity);
        });

    m_Input->flash->set_Callback(
        [this](Action a)
        {
            if (a == Action::PRESSED)
            {
                Vector<double, 2> flash_Dir = {0, 0};
                if (m_Input->left->get_State() == State::PRESSED)
                    flash_Dir[0] -= 60;
                if (m_Input->right->get_State() == State::PRESSED)
                    flash_Dir[0] += 60;
                if (m_Input->up->get_State() == State::PRESSED)
                    flash_Dir[1] -= 60;
                if (m_Input->down->get_State() == State::PRESSED)
                    flash_Dir[1] += 60;
                m_Entity.get<TransformComponent>()->position += flash_Dir;
                m_Entity.get<DynamicPhysicsComponent>()->velocity = 0;
            }
        });

    m_Input->up->set_Callback(
        [this](Action a)
        {
            if (a == Action::PRESSED)
            {
                if (ground)
                    m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -420;
                else if (jumps)
                {
                    m_Entity.get<DynamicPhysicsComponent>()->velocity[1] = -330;
                    jumps--;
                }
            }
        });

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

void EisScript::post_Input(void)
{
    auto transform = m_Entity.get<TransformComponent>();
    auto physics = m_Entity.get<DynamicPhysicsComponent>();

    ground = false;

    physics->velocity[0] = 0;

    auto axis_Value = m_Input->x_Axis->get_Value();

    if (axis_Value < -0.2)
    {
        physics->velocity[0] += 180 * axis_Value;
        transform->state = 0;
    }

    if (axis_Value > 0.2)
    {
        physics->velocity[0] += 180 * axis_Value;
        transform->state = GEAR_MIRROR_X;
    }

    m_Entity.update_Transformation();
}