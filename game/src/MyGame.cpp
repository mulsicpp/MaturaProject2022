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
#include "scripts/BaseFighterScript.h"

#include <gear/input/Input.h>

#include <gear/renderer/Renderer.h>
#include <gear/renderer/SpriteComponent.h>
#include <gear/renderer/AnimationComponent.h>

#include <gear/resource/ResourceManager.h>
#include <gear/resource/Sprite.h>
#include <gear/resource/Animation.h>
#include <gear/resource/Palette.h>

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
        if (e.get_Other_Entity().get_Entity_ID() == 0 && Input::get_Key_State(Key::S) == State::PRESSED)
            return false;
        if (e.get_Other_Entity().get_Entity_ID() == 1 && Input::get_Axis_Value(0, ControllerAxis::LEFT_STICK_Y) > 0.4)
            return false;
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

Ref<Shape> get_Random_Shape() {
    return Ref<Shape>(new Rect({0, 0}, {1, 1}));
}

void MyGame::on_Startup(void)
{
    gear::Game::on_Startup();

    window->set_Fullscreen();

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

    SpriteComponent sprite_Comp;
    sprite_Comp.offset = {0, 0};
    sprite_Comp.parallax_Factor = 1;
    sprite_Comp.sprite = ResourceManager::get<Sprite>("assets/test_sprites/eis.gear");
    sprite_Comp.palette = palettes[0];

    AnimationComponent animation_Comp;
    animation_Comp.offset = {-32, -32, 0};
    animation_Comp.parallax_Factor = 1;
    animation_Comp.palette = palettes[0];
    animation_Comp.type = AnimationType::LOOP;
    animation_Comp.animation = ResourceManager::get<Animation>("assets/test_sprites/eis_idle.gear");
    animation_Comp.frame_Offset = 0;
    // animation_Comp.on_Frame_Changed = [](int offset) {
    //     GEAR_DEBUG_LOG("frame offset: %i", offset);
    // };
    animation_Comp.frame_Rate = animation_Comp.animation->get_Default_Frame_Rate();

    Entity new_Eis = main_Scene->create_Entity();
    GEAR_DEBUG_LOG("created first entity");
    // GEAR_DEBUG_LOG("created entity %i %i %p", j, i, new_Eis);
    

    animation_Comp.parallax_Factor = 1.0f;
    animation_Comp.offset = {-32, -32, 0};

    animation_Comp.palette = palettes[1];
    new_Eis.add<AnimationComponent>({animation_Comp});
    new_Eis.add<TransformComponent>({{-100, -40}, {1, 1}, GEAR_MIRROR_X});
    new_Eis.add<DynamicPhysicsComponent>({Collider::create(
                                              Rect{{-12, 14}, {12, 32}},
                                              Circle{{2, 14}, 9},
                                              Circle{{7, 5}, 6},
                                              Circle{{11, -3}, 3}),
                                          0,
                                          eis_Physics_Check,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          {0, 0},
                                          {0, 1200},
                                          {-10000, 10000},
                                          {-1200, 360},
                                          10});

    new_Eis.add<HurtboxComponent>({{Hurtbox::create(1, Circle{{0, 23}, 12})}});
    HitboxComponent h = {{Hitbox::create(1, Circle{{-20, 23}, 12, false})}};
    h.hitboxes[0]->on_Collision_Begin([](HitboxEvent e) { 
        e.get_Other_Entity().get<TransformComponent>()->position[0] += e.get_Entity().get<TransformComponent>()->state & GEAR_MIRROR_X ? 20 : -20; });
    new_Eis.add<HitboxComponent>(h);
    GEAR_DEBUG_LOG("pre script");
    new_Eis.add<ScriptComponent>(ScriptComponent().bind<EisScript>(InputDevice::KEYBOARD));
    GEAR_DEBUG_LOG("post script");

    animation_Comp.palette = palettes[3];
    animation_Comp.frame_Offset = 1;

    new_Eis = main_Scene->create_Entity();
    new_Eis.add<AnimationComponent>(animation_Comp);
    new_Eis.add<TransformComponent>({{100, -40}, {1, 1}, 0});
    // physics.collider
    new_Eis.add<DynamicPhysicsComponent>({Collider::create(
                                              Rect{{-12, 14}, {12, 32}},
                                              Circle{{2, 14}, 9},
                                              Circle{{7, 5}, 6},
                                              Circle{{11, -3}, 3}),
                                          0,
                                          eis_Physics_Check,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          {0, 0},
                                          {0, 1000},
                                          {-10000, 10000},
                                          {-1200, 360},
                                          10});

    new_Eis.add<HurtboxComponent>({{Hurtbox::create(1, Circle{{0, 23}, 12})}});
    h = {{Hitbox::create(1, Circle{{-20, 23}, 12, false})}};
    h.hitboxes[0]->on_Collision_Begin([](HitboxEvent e) { e.get_Other_Entity().get<DynamicPhysicsComponent>()->velocity = {0, -4}; });
    new_Eis.add<HitboxComponent>(h);
    new_Eis.add<ScriptComponent>(ScriptComponent().bind<EisScript>(InputDevice::CONTROLLER_1));

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

    Entity base_Fighter = main_Scene->create_Entity();
    base_Fighter.add<ScriptComponent>(ScriptComponent().bind<BaseFighterScript>(InputDevice::KEYBOARD));

    main_Scene->update_Transformation();

    GEAR_DEBUG_LOG("finished scene");

    cam_Pos = {0, 0};

    Renderer::set_Camera(&cam);
}

void MyGame::render(void) {
    cam_Pos = (Entity{0, 0}.get<TransformComponent>()->position + Entity{1, 0}.get<TransformComponent>()->position) / 2;

    cam.follow_Target();
    Renderer::start_New_Frame();
    Renderer::render_Scene(main_Scene);
    //Renderer::render_All_Hitboxes(main_Scene);
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