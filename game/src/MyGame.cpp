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

#include "scripts/EisScript.h"

using namespace gear;

void MyGame::on_Startup(void)
{
  m_Window->set_Title("Game Window!");
  m_Window->set_Resizable(false);
  m_Window->set_Size(1280, 720);
  //m_Window->set_Fullscreen();
  m_Window->set_Visible(true);

  Renderer::create(640, 360);
  Renderer::set_V_Sync(true);


  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  s1 = Ref<Shape>(new Circle({0, 0}, 20));
  s2 = Ref<Shape>(new Rect({-50, -30}, {50, 30}));


  Input::add_Global_Callback<ControllerButtonEvent>([](ControllerButtonEvent e)
  {
    GEAR_DEBUG_LOG("gamepad button %s", e.get_Action() == Action::PRESSED ? "pressed" : "released");
  });
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
  animation_Comp.animation = ResourceManager::get<Animation>("assets/test_sprites/eis_jumping_besser.gear");
  animation_Comp.animation_Offset = 0;
  animation_Comp.frame_Rate = animation_Comp.animation->get_Default_Frame_Rate();

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 10; j++)
    {
      Entity new_Eis = m_Scene->create_Entity();
      //GEAR_DEBUG_LOG("created entity %i %i %p", j, i, new_Eis);

      animation_Comp.palette = palettes[(i * 13 + j) % 7];
      animation_Comp.animation_Offset++;
      if (animation_Comp.animation_Offset >= animation_Comp.animation->get_Frame_Count())
        animation_Comp.animation_Offset = 0;

      animation_Comp.parallax_Factor = 1.0f / (i * 0.5 + 1);
      animation_Comp.offset = {-32, -32, 1.0f / (i * 0.5 + 1)};
      new_Eis.add<AnimationComponent>(animation_Comp);
      Vector<double, 2> pos(32 - 320 + j * 64.0f, 0);
      new_Eis.add<TransformComponent>({pos, {1, 1}, GEAR_MIRROR_X});
    }
  Entity entity = m_Scene->get_Entity(7);
  entity.add<EventComponent<KeyEvent>>({[](KeyEvent e) {
    GEAR_DEBUG_LOG("key pressed");
  }});

  //m_Scene->print();

  GEAR_DEBUG_LOG("finished scene");

  cam_Pos = {0, 0};

  Renderer::set_Camera(&cam);
}

void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  Input::dispatch_Events(m_Scene);

  call_Script_Update(m_Scene);
  if (Input::get_Key_State(Key::A) == State::PRESSED || Input::get_Key_State(Key::LEFT) == State::PRESSED)
    cam_Pos[0] -= 6;

  if (Input::get_Key_State(Key::D) == State::PRESSED || Input::get_Key_State(Key::RIGHT) == State::PRESSED)
    cam_Pos[0] += 6;

  if (Input::get_Key_State(Key::W) == State::PRESSED || Input::get_Key_State(Key::UP) == State::PRESSED)
    cam_Pos[1] -= 6;

  if (Input::get_Key_State(Key::S) == State::PRESSED || Input::get_Key_State(Key::DOWN) == State::PRESSED)
    cam_Pos[1] += 6;

  cam.follow_Target();
  Renderer::start_New_Frame();
  Renderer::render_Scene(m_Scene);
  ((Circle *)s1.get())->position = Input::get_Cursor_Position().cast_To<double, 2>() / 2 - Vector<double, 2>{320, 180};
  //((Rect *)s1.get())->bottom_Right = ((Rect *)s1.get())->top_Left + Vector<double, 2>{50, 30};
  //((Circle *)s1.get())->position += {0, 0.7};
  Vector<double, 2> sep_Vec(0, 0);
  bool intersects = s2->intersects(s1.get(), &sep_Vec);
  if(intersects)
  {
    ((Circle *)s1.get())->position += sep_Vec;
  }
  Renderer::render_Shape(s2.get(), intersects ? Vector<float, 4>{1, 0, 0, 1} : Vector<float, 4>{0, 0, 1, 1});
  Renderer::render_Shape(s1.get(), {0, 1, 0, 1});
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