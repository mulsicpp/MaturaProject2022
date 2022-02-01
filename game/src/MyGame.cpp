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

using namespace gear;

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

  EventComponent<KeyEvent> e = [](KeyEvent e){
  };
  Input::add_Global_Callback<ControllerButtonEvent>([](ControllerButtonEvent e){GEAR_DEBUG_LOG("gamepad button %s", e.get_Action() == Action::PRESSED ? "pressed" : "released");});
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
  // animation_Comp.palette = ResourceManager::get<Palette>("assets/test_sprites/kirby_walk_palette.gear");
  // animation_Comp.animation = ResourceManager::get<Animation>("assets/test_sprites/kirby_walk.gear");
  animation_Comp.animation_Offset = 0;
  animation_Comp.frame_Rate = animation_Comp.animation->get_Default_Frame_Rate();

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 10; j++)
    {
      // GEAR_DEBUG_LOG("creating entity");
      Entity *new_Eis = m_Scene->create_Entity();
      GEAR_DEBUG_LOG("created entity %i %i %p", j, i, new_Eis);

      animation_Comp.palette = palettes[(i * 13 + j) % 7];
      animation_Comp.animation_Offset++;
      if (animation_Comp.animation_Offset >= animation_Comp.animation->get_Frame_Count())
        animation_Comp.animation_Offset = 0;

      animation_Comp.parallax_Factor = 1.0f/(i + 1);
      animation_Comp.offset = {-32, -32, 1.0f/(i + 1)};
      // GEAR_DEBUG_LOG("about to add animation");
      new_Eis->add<AnimationComponent>(animation_Comp);
      // GEAR_DEBUG_LOG("added animation");
      Vector<float, 2> pos(32 - 320 + j * 64.0f, 32 - 180 + i * 64.0f);
      new_Eis->add<TransformComponent>({pos, {1, 1}, 0});
      // GEAR_DEBUG_LOG("added position");
    }

  GEAR_DEBUG_LOG("finished scene");

  cam_Pos = {0, 0};

  Renderer::set_Camera(&cam);
}


void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  Input::dispatch_Events(nullptr);
  /*
    GEAR_DEBUG_LOG("eis %p", eis);
    auto *pos_Comp = eis->get<TransformComponent>();
    GEAR_DEBUG_LOG("got pos comp %p", pos_Comp);
    auto *pos = &(pos_Comp->data.position);
    GEAR_DEBUG_LOG("got position %p", pos);
    (*pos)[0] -= 2;
    if((*pos)[0] < -64){
      GEAR_DEBUG_LOG("if statement");
      (*pos)[0] = 640;
      palette_Index = (palette_Index + 1) % 3;
      eis->get<SpriteComponent>()->data.palette = palettes[palette_Index];
      GEAR_DEBUG_LOG("set palette");
    }
  */
  // GEAR_DEBUG_LOG("start frame");
  if(Input::get_Key_State(Key::A) == State::PRESSED || Input::get_Key_State(Key::LEFT) == State::PRESSED)
    cam_Pos[0] -= 2;
  
  if(Input::get_Key_State(Key::D) == State::PRESSED || Input::get_Key_State(Key::RIGHT) == State::PRESSED)
    cam_Pos[0] += 2;

  if(Input::get_Key_State(Key::W) == State::PRESSED || Input::get_Key_State(Key::UP) == State::PRESSED)
    cam_Pos[1] -= 2;

  if(Input::get_Key_State(Key::S) == State::PRESSED || Input::get_Key_State(Key::DOWN) == State::PRESSED)
    cam_Pos[1] += 2;
  

  cam.follow_Target();
  Renderer::start_New_Frame();
  // GEAR_DEBUG_LOG("render scene");
  Renderer::render_Scene(m_Scene);
  // GEAR_DEBUG_LOG("show frame");
  Renderer::show_Frame();
}

void MyGame::on_Shutdown(void)
{
  GEAR_DEBUG_LOG("destroy scene");
  Renderer::destroy();
  m_Scene->destroy();
  for (int i = 0; i < 7; i++)
    palettes[i] = nullptr;
  GEAR_DEBUG_LOG("unloaded resources: %i", ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}