#include "MyGame.h"

#include <gear/scripting/ScriptComponent.h>

#include <time.h>

#include <gear/collision/shapes/Rect.h>
#include <gear/collision/shapes/Circle.h>
#include <gear/collision/shapes/Point.h>
#include <gear/collision/DynamicPhysicsComponent.h>
#include <gear/collision/HitboxComponent.h>
#include <gear/collision/HurtboxComponent.h>

#include "scripts/EisScript.h"
#include "scripts/MedusaScript.h"
#include "scripts/BaseFighterScript.h"

#include <gear/input/Input.h>

#include <gear/renderer/Renderer.h>
#include <gear/renderer/SpriteComponent.h>
#include <gear/renderer/AnimationComponent.h>
#include <gear/renderer/TextComponent.h>

#include <gear/resource/ResourceManager.h>
#include <gear/resource/Sprite.h>
#include <gear/resource/Animation.h>
#include <gear/resource/Palette.h>

#include "components/FlagComponent.h"

#include <gear/input/events/TextEvent.h>

using namespace gear;

MyCamera::MyCamera(const Vector<double, 2> *target_Position) : gear::Camera(target_Position) {}

void MyCamera::follow_Target(void)
{
    this->m_Position += (*m_Target_Position - m_Position) / 20;
}

bool platform_Physics_Check(gear::CollisionEvent e)
{
    if (!e.did_Intersect())
    {
        Entity other = e.get_Other_Entity();
        auto flags = other.get<FlagComponent>()->flags;
        if(flags & FLAG_PROJECTILE){
            return false;
        }
        else if (flags & FLAG_FIGHTER)
        {
            if (((BaseFighterScript *)other.get<ScriptComponent>()->script)->is_Phasing())
                return false;
        }
        if (abs(e.get_Separation_Vector()[0]) < abs(e.get_Separation_Vector()[1]) / 20 && e.get_Separation_Vector()[1] < 0)
            return true;
    }
    return false;
}

bool eis_Physics_Check(gear::CollisionEvent e)
{
    if (e.get_Other_Entity().get_Entity_ID() == 0 || e.get_Other_Entity().get_Entity_ID() == 1)
        return false;
    return true;
}

void construct_Entity(Entity e)
{
    e.add<TransformComponent>({{0, 0}, {1, 1}, 0});
    e.add<FlagComponent>({0});
}

class X {
public:
    void x(int i) {
        return;
    }
};

void MyGame::on_Startup(void)
{
    gear::Game::on_Startup();

    main_Scene->default_Entity_Contruction(construct_Entity);

    Component<FlagComponent>::allow();

    window->set_Fullscreen();

    GEAR_DEBUG_LOG("width: %i height: %i", window->get_Width(), window->get_Height());

    Renderer::set_Viewport({0, 0}, {window->get_Width(), window->get_Height()});
    Renderer::set_Clear_Color({0.1, 0.7, 1, 1});

    GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
    GEAR_DEBUG_LOG("Opened application");

    SpriteComponent sprite;
    sprite.parallax_Factor = 1;
    sprite.palette = ResourceManager::get<Palette>("assets/stages/floating_island/main_palette.gear");
    sprite.sprite = ResourceManager::get<Sprite>("assets/stages/floating_island/main_stage_bg.gear");
    sprite.offset = {-sprite.sprite->get_Width() / 2., -sprite.sprite->get_Height() / 2., -0.1};

    Entity stage = main_Scene->create_Entity();
    stage.add<TransformComponent>({{0, 0}, {1, 1}, 0});
    stage.add<SpriteComponent>(sprite);

    StaticPhysicsComponent physics;
    physics.collider = Collider::create(Rect{{-sprite.sprite->get_Width() / 2. + 16, -sprite.sprite->get_Height() / 2. + 250}, {sprite.sprite->get_Width() / 2. - 10, -sprite.sprite->get_Height() / 2. + 290}});
    physics.check = gear::default_Physics_Check;
    physics.restitution = 1;
    stage.add<StaticPhysicsComponent>(physics);

    physics.check = platform_Physics_Check;

    sprite.sprite = ResourceManager::get<Sprite>("assets/stages/floating_island/main_stage_fg.gear");
    sprite.offset = {-sprite.sprite->get_Width() / 2., -sprite.sprite->get_Height() / 2., 0.1};

    Entity stage2 = main_Scene->create_Entity();
    stage2.add<TransformComponent>({{0, 0}, {1, 1}, 0});
    stage2.add<SpriteComponent>(sprite);
    //physics.collider = Collider::create(Rect{{-sprite.sprite->get_Width() / 2. + 250, -sprite.sprite->get_Height() / 2. + 11}, {-sprite.sprite->get_Width() / 2. + 280, sprite.sprite->get_Height() / 2. - 7}});
    //platform2.add<StaticPhysicsComponent>(physics);

    p1 = main_Scene->create_Entity();
    p1.add<ScriptComponent>(ScriptComponent().bind<EisScript>(InputDevice::KEYBOARD, "vanilla.gear", 0));
    p1.set<TransformComponent>({{-100, -30}});

    p2 = main_Scene->create_Entity();
    p2.add<ScriptComponent>(ScriptComponent().bind<MedusaScript>(InputDevice::CONTROLLER_1, "default.gear", 1));
    p2.set<TransformComponent>({{100, -30}});

    StaticPhysicsComponent physics2;
    physics2.collider = Collider::create(Rect({0, 0}, {100, 6}));
    physics2.check = platform_Physics_Check;

    Entity platform1 = main_Scene->create_Entity();
    platform1.set<TransformComponent>({{-sprite.sprite->get_Width() / 2 + 135., -sprite.sprite->get_Height() / 2 + 185.}, {1, 1}, 0});
    platform1.add<StaticPhysicsComponent>(physics2);

    physics2.collider = Collider::create(Rect({0, 0}, {100, 6}));

    Entity platform2 = main_Scene->create_Entity();
    platform2.set<TransformComponent>({{-sprite.sprite->get_Width() / 2 + 60., -sprite.sprite->get_Height() / 2 + 118.}, {1, 1}, 0});
    platform2.add<StaticPhysicsComponent>(physics2);

    physics2.collider = Collider::create(Rect({0, 0}, {100, 6}));

    Entity platform3 = main_Scene->create_Entity();
    platform3.set<TransformComponent>({{-sprite.sprite->get_Width() / 2 + 267., -sprite.sprite->get_Height() / 2 + 154.}, {1, 1}, 0});
    platform3.add<StaticPhysicsComponent>(physics2);

    main_Scene->update_Transformation();

    GEAR_DEBUG_LOG("finished scene");

    cam_Pos = {0, 0};

    Renderer::set_Camera(&cam);
}

void MyGame::render(void)
{
    cam_Pos = (p1.get<TransformComponent>()->position + p2.get<TransformComponent>()->position) / 2;

    cam.follow_Target();
    
    Renderer::start_New_Frame();
    Renderer::render_Scene(main_Scene);
    // Renderer::render_All_Hitboxes(main_Scene);
    Renderer::show_Frame();
}

void MyGame::on_Shutdown(void)
{
    gear::Game::on_Shutdown();
    for (int i = 0; i < 7; i++)
        palettes[i] = nullptr;
    GEAR_DEBUG_LOG("unloaded resources: %i", ResourceManager::unload());
    GEAR_DEBUG_LOG("Closed application");
}