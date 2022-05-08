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

#include <gear/ui/UI.h>

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
    sprite.parallax_Factor = 1;
    sprite.palette = ResourceManager::get<Palette>("assets/stages/floating_island/main_palette.gear");
    sprite.sprite = ResourceManager::get<Sprite>("assets/stages/floating_island/main_stage_bg.gear");
    sprite.offset = {-sprite.sprite->get_Width() / 2., -sprite.sprite->get_Height() / 2., -0.1};

    Entity platform = main_Scene->create_Entity();
    platform.add<TransformComponent>({{0, 0}, {1, 1}, 0});
    platform.add<SpriteComponent>(sprite);

    StaticPhysicsComponent physics;
    physics.collider = Collider::create(Rect{{-sprite.sprite->get_Width() / 2. + 16, -sprite.sprite->get_Height() / 2. + 250}, {sprite.sprite->get_Width() / 2. - 10, -sprite.sprite->get_Height() / 2. + 290}});
    physics.check = gear::default_Physics_Check;
    physics.restitution = 1;
    platform.add<StaticPhysicsComponent>(physics);

    physics.check = platform_Physics_Check;

    sprite.sprite = ResourceManager::get<Sprite>("assets/stages/floating_island/main_stage_fg.gear");
    sprite.offset = {-sprite.sprite->get_Width() / 2., -sprite.sprite->get_Height() / 2., 0.1};

    Entity platform2 = main_Scene->create_Entity();
    platform2.add<TransformComponent>({{0, 0}, {1, 1}, 0});
    platform2.add<SpriteComponent>(sprite);
    //physics.collider = Collider::create(Rect{{-sprite.sprite->get_Width() / 2. + 250, -sprite.sprite->get_Height() / 2. + 11}, {-sprite.sprite->get_Width() / 2. + 280, sprite.sprite->get_Height() / 2. - 7}});
    //platform2.add<StaticPhysicsComponent>(physics);

    medusa = main_Scene->create_Entity();
    medusa.add<ScriptComponent>(ScriptComponent().bind<MedusaScript>(InputDevice::KEYBOARD));

    main_Scene->update_Transformation();

    GEAR_DEBUG_LOG("finished scene");

    cam_Pos = {0, 0};

    Renderer::set_Camera(&cam);

    Ref<UIContainer> main_Container = UIContainer::create();

    main_Container->add_Component<UIComponent>({{{0, 0}}, gear::ResourceManager::get<Animation>("assets/ui/test_button/default.gear"), gear::ResourceManager::get<Palette>("assets/ui/test_button/palette.gear"),0});

    UI::set_Container(main_Container);
}

void MyGame::render(void)
{
    cam_Pos = medusa.get<TransformComponent>()->position;

    cam.follow_Target();
    Renderer::start_New_Frame();
    Renderer::render_Scene(main_Scene);
    // Renderer::render_All_Hitboxes(main_Scene);
    Renderer::render_UI();
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