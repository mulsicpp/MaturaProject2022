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
        if (other.get<FlagComponent>()->flags & FLAG_FIGHTER)
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

void MyGame::on_Startup(void)
{
    gear::Game::on_Startup();

    main_Scene->default_Entity_Contruction(construct_Entity);

    Component<FlagComponent>::allow();

    // window->set_Fullscreen();

    GEAR_DEBUG_LOG("width: %i height: %i", window->get_Width(), window->get_Height());

    Renderer::set_Viewport({0, 0}, {window->get_Width(), window->get_Height()});
    Renderer::set_Clear_Color({0.1, 0.7, 1, 1});

    GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
    GEAR_DEBUG_LOG("Opened application");

    palettes[0] = ResourceManager::get<Palette>("assets/test_sprites/eis_palette_yellow.gear");
    palettes[1] = ResourceManager::get<Palette>("assets/test_sprites/eis_palette_pink.gear");
    palettes[2] = ResourceManager::get<Palette>("assets/test_sprites/eis_palette_green.gear");
    palettes[3] = ResourceManager::get<Palette>("assets/test_sprites/eis_palette_dark.gear");
    palettes[4] = ResourceManager::get<Palette>("assets/test_sprites/eis_palette_blue.gear");
    palettes[5] = ResourceManager::get<Palette>("assets/test_sprites/eis_palette_vanillia.gear");
    palettes[6] = ResourceManager::get<Palette>("assets/test_sprites/eis_palette_brown.gear");

    SpriteComponent sprite;
    sprite.offset = {-183, -3, 0.1};
    sprite.parallax_Factor = 1;
    sprite.palette = ResourceManager::get<Palette>("assets/test_sprites/platform_palette.gear");
    sprite.sprite = ResourceManager::get<Sprite>("assets/test_sprites/platform.gear");

    Entity platform = main_Scene->create_Entity();
    platform.add<TransformComponent>({{0, 80}, {1, 1}, 0});
    platform.add<SpriteComponent>(sprite);

    StaticPhysicsComponent physics;
    physics.collider = Collider::create(Rect{{-183, 0}, {183, 20}});
    physics.check = gear::default_Physics_Check;
    physics.restitution = 1;
    platform.add<StaticPhysicsComponent>(physics);

    physics.check = platform_Physics_Check;

    sprite.offset = {-35, -3, 0.1};
    sprite.sprite = ResourceManager::get<Sprite>("assets/test_sprites/platform2.gear");

    Entity platform2 = main_Scene->create_Entity();
    platform2.add<TransformComponent>({{-100, 20}, {1, 1}, 0});
    platform2.add<SpriteComponent>(sprite);
    physics.collider = Collider::create(Rect{{-35, 0}, {35, 5}});
    platform2.add<StaticPhysicsComponent>(physics);

    Entity platform3 = main_Scene->create_Entity();
    platform3.add<TransformComponent>({{100, 20}, {1, 1}, 0});
    platform3.add<SpriteComponent>(sprite);
    physics.collider = Collider::create(Rect{{-35, 0}, {35, 5}});
    platform3.add<StaticPhysicsComponent>(physics);

    Entity platform4 = main_Scene->create_Entity();
    platform4.add<TransformComponent>({{0, -30}, {1, 1}, 0});
    platform4.add<SpriteComponent>(sprite);
    physics.collider = Collider::create(Rect{{-35, 0}, {35, 5}});
    platform4.add<StaticPhysicsComponent>(physics);

    medusa = main_Scene->create_Entity();
    medusa.add<ScriptComponent>(ScriptComponent().bind<MedusaScript>(InputDevice::KEYBOARD));

    Entity text_Entity = main_Scene->create_Entity();
    text_Entity.set<TransformComponent>({{0, 0, 0}, {2, 2}, 0});

    TextComponent text;
    text.font = ResourceManager::get<Font>("assets/fonts/font1.gear");
    text.text = "Hello";
    text.offset = {-100, -11, 0.5};
    text.raw_Text = true;
    text.colors = text.font->get_Colors();
    text.colors[0] = {0, 127, 255, 255};
    text.colors[1] = {0, 63, 127, 255};
    text_Entity.add<TextComponent>(text);

    main_Scene->update_Transformation();

    GEAR_DEBUG_LOG("finished scene");

    cam_Pos = {0, 0};

    Renderer::set_Camera(&cam);
}

void MyGame::render(void)
{
    cam_Pos = medusa.get<TransformComponent>()->position;

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