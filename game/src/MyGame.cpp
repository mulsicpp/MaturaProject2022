#include "MyGame.h"

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>
#include <gear/renderer/Renderer.h>
#include <gear/renderer/SpriteComponent.h>
#include <gear/renderer/AnimationComponent.h>

#include <gear/scene/TransformComponent.h>
#include <exception>

#include <gear/event/event_Types/KeyEvent.h>
#include <gear/event/EventComponent.h>
#include <gear/event/Input.h>

#include <gear/scripting/ScriptComponent.h>

#include <time.h>

#include <gear/collision/shapes/Rect.h>
#include <gear/collision/shapes/Circle.h>
#include <gear/collision/shapes/Point.h>
#include <gear/collision/DynamicPhysicsComponent.h>

#include "scripts/EisScript.h"
#include "scripts/EisScript2.h"

using namespace gear;

MyCamera::MyCamera(const Vector<double, 2> *target_Position) : gear::Camera(target_Position) {}

void MyCamera::follow_Target(void) {
  this->m_Position += (*m_Target_Position - m_Position) / 20;
}

bool platform_Physics_Check(gear::Vector<double, 2> push_Direction, bool pre_Intersect, Entity entity1, Entity entity2)
{
  if (!pre_Intersect)
  {
    if (entity2.get_Entity_ID() == 0 && Input::get_Key_State(Key::S) == State::PRESSED)
      return false;
    if (entity2.get_Entity_ID() == 1 && Input::get_Axis_Value(0, ControllerAxis::LEFT_STICK_Y) > 0.4)
      return false;
    if (abs(push_Direction[0]) < abs(push_Direction[1]) / 20 && push_Direction[1] < 0)
      return true;
  }
  return false;
}

bool eis_Physics_Check(gear::Vector<double, 2> push_Direction, bool pre_Intersect, Entity entity1, Entity entity2)
{
  if (entity2.get_Entity_ID() == 0 || entity2.get_Entity_ID() == 1)
    return false;
  return true;
}

void MyGame::on_Startup(void)
{
  m_Window->set_Title("Game Window!");
  m_Window->set_Resizable(false);
  m_Window->set_Size(1280, 720);
  // m_Window->set_Fullscreen();
  m_Window->set_Visible(true);

  Renderer::create(640, 360);
  Renderer::set_V_Sync(true);

  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  Input::add_Global_Callback<ControllerButtonEvent>([](ControllerButtonEvent e)
                                                    { GEAR_DEBUG_LOG("gamepad button %s", e.get_Action() == Action::PRESSED ? "pressed" : "released"); });
  GEAR_DEBUG_LOG("%s", glfwGetVersionString());

  allow_Gear_Components();

  m_Scene = Scene::get(0);
  m_Scene->create();

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
  animation_Comp.animation = ResourceManager::get<Animation>("assets/test_sprites/eis_idle.gear");
  animation_Comp.animation_Offset = 0;
  animation_Comp.frame_Rate = animation_Comp.animation->get_Default_Frame_Rate();

  Entity new_Eis = m_Scene->create_Entity();
  // GEAR_DEBUG_LOG("created entity %i %i %p", j, i, new_Eis);

  animation_Comp.palette = palettes[5];
  animation_Comp.animation_Offset++;
  if (animation_Comp.animation_Offset >= animation_Comp.animation->get_Frame_Count())
    animation_Comp.animation_Offset = 1.0f;

  animation_Comp.parallax_Factor = 1.0f;
  animation_Comp.offset = {-32, -32, 0};
  new_Eis.add<AnimationComponent>(animation_Comp);
  new_Eis.add<TransformComponent>({{0, -40}, {1, 1}, 0});
  new_Eis.add<DynamicPhysicsComponent>({Hitbox::create(
                                     Rect{{-12, 14}, {12, 32}},
                                     Circle{{2, 14}, 9},
                                     Circle{{7, 5}, 6},
                                     Circle{{11, -3}, 3}),
                                 0,
                                 eis_Physics_Check,
                                 nullptr,
                                 nullptr,
                                 {0, 0},
                                 {0, 0.3},
                                 {-100, 100},
                                 {-20, 6},
                                 10});

  new_Eis.add<ScriptComponent>(ScriptComponent().bind<EisScript>());

  animation_Comp.palette = palettes[6];

  new_Eis = m_Scene->create_Entity();
  new_Eis.add<AnimationComponent>(animation_Comp);
  new_Eis.add<TransformComponent>({{100, -40}, {1, 1}, 0});
  // physics.collider
  new_Eis.add<DynamicPhysicsComponent>({Hitbox::create(
                                     Rect{{-12, 14}, {12, 32}},
                                     Circle{{2, 14}, 9},
                                     Circle{{7, 5}, 6},
                                     Circle{{11, -3}, 3}),
                                 0,
                                 eis_Physics_Check,
                                 nullptr,
                                 nullptr,
                                 {0, 0},
                                 {0, 0.3},
                                 {-100, 100},
                                 {-20, 6},
                                 10});

  new_Eis.add<ScriptComponent>(ScriptComponent().bind<EisScript2>());

  SpriteComponent sprite;
  sprite.offset = {-183, -3, 0.1};
  sprite.parallax_Factor = 1;
  sprite.palette = ResourceManager::get<Palette>("assets/test_sprites/platform_palette.gear");
  sprite.sprite = ResourceManager::get<Sprite>("assets/test_sprites/platform.gear");

  Entity platform = m_Scene->create_Entity();
  platform.add<TransformComponent>({{0, 80}, {1, 1}, 0});
  platform.add<SpriteComponent>(sprite);

  StaticPhysicsComponent physics;
  physics.collider = Hitbox::create(Rect{{-183, 0}, {183, 20}});
  physics.check = gear::default_Physics_Check;
  platform.add<StaticPhysicsComponent>(physics);

  physics.check = platform_Physics_Check;

  sprite.offset = {-35, -3, 0.1};
  sprite.sprite = ResourceManager::get<Sprite>("assets/test_sprites/platform2.gear");

  Entity platform2 = m_Scene->create_Entity();
  platform2.add<TransformComponent>({{-100, 20}, {1, 1}, 0});
  platform2.add<SpriteComponent>(sprite);
  physics.collider = Hitbox::create(Rect{{-35, 0}, {35, 5}});
  platform2.add<StaticPhysicsComponent>(physics);

  Entity platform3 = m_Scene->create_Entity();
  platform3.add<TransformComponent>({{100, 20}, {1, 1}, 0});
  platform3.add<SpriteComponent>(sprite);
  //physics.collider = Hitbox::create(Rect{{-35, 0}, {35, 5}});
  platform3.add<StaticPhysicsComponent>(physics);

  Entity platform4 = m_Scene->create_Entity();
  platform4.add<TransformComponent>({{0, -30}, {1, 1}, 0});
  platform4.add<SpriteComponent>(sprite);
  // physics.collider = Hitbox::create(Rect{{-35, 0}, {35, 5}});
  platform4.add<StaticPhysicsComponent>(physics);

  GEAR_DEBUG_LOG("finished scene");

  cam_Pos = {0, 0};

  Renderer::set_Camera(&cam);
}

void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  Input::dispatch_Events(m_Scene);

  m_Scene->update_Transformation();
  call_Script_Update(m_Scene);

  physics_Step(m_Scene);
  physics_Step(m_Scene);
  physics_Step(m_Scene);

  cam_Pos = (Entity{0, 0}.get<TransformComponent>()->position + Entity{1, 0}.get<TransformComponent>()->position) / 2;

  cam.follow_Target();
  Renderer::start_New_Frame();
  Renderer::render_Scene(m_Scene);
  Renderer::render_All_Hitboxes(m_Scene);
  Renderer::show_Frame();
}

void MyGame::on_Shutdown(void)
{
  Renderer::destroy();
  GEAR_DEBUG_LOG("destroy scene");
  m_Scene->destroy();
  for (int i = 0; i < 7; i++)
    palettes[i] = nullptr;
  GEAR_DEBUG_LOG("unloaded resources: %i", ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}