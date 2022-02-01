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

void MyGame::on_Startup(void)
{
  m_Window->set_Title("Game Window!");
  m_Window->set_Resizable(false);
  m_Window->set_Size(1280, 720);
  // m_Window->set_Fullscreen();
  m_Window->set_Visible(true);

  gear::Renderer::create(640, 360);
  gear::Renderer::set_V_Sync(true);

  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  gear::EventComponent<gear::KeyEvent> e = {[](gear::KeyEvent e){}, false};
  gear::Input::add_Global_Callback<gear::ControllerButtonEvent>([](gear::ControllerButtonEvent e){GEAR_DEBUG_LOG("gamepad button %s", e.get_Action() == gear::Action::PRESSED ? "pressed" : "released");});
  GEAR_DEBUG_LOG("%s", glfwGetVersionString());

  gear::allow_Gear_Components();

  m_Scene = gear::Scene::get(0);
  m_Scene->create();

  palettes[0] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_yellow.gear");
  palettes[1] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_pink.gear");
  palettes[2] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_green.gear");
  palettes[3] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_dark.gear");
  palettes[4] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_blue.gear");
  palettes[5] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_vanillia.gear");
  palettes[6] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_brown.gear");

  gear::SpriteComponent sprite_Comp;
  sprite_Comp.offset = {0, 0};
  sprite_Comp.parallax_Factor = 1;
  sprite_Comp.sprite = gear::ResourceManager::get<gear::Sprite>("assets/test_sprites/eis.gear");
  sprite_Comp.palette = palettes[0];

  gear::AnimationComponent animation_Comp;
  animation_Comp.offset = {-32, -32, 0};
  animation_Comp.parallax_Factor = 1;
  animation_Comp.palette = palettes[0];
  animation_Comp.animation = gear::ResourceManager::get<gear::Animation>("assets/test_sprites/eis_idle.gear");
  // animation_Comp.palette = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/kirby_walk_palette.gear");
  // animation_Comp.animation = gear::ResourceManager::get<gear::Animation>("assets/test_sprites/kirby_walk.gear");
  animation_Comp.animation_Offset = 0;
  animation_Comp.frame_Rate = animation_Comp.animation->get_Default_Frame_Rate();

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 10; j++)
    {
      // GEAR_DEBUG_LOG("creating entity");
      gear::Entity *new_Eis = m_Scene->create_Entity();
      GEAR_DEBUG_LOG("created entity %i %i %p", j, i, new_Eis);

      animation_Comp.palette = palettes[(i * 13 + j) % 7];
      animation_Comp.animation_Offset++;
      if (animation_Comp.animation_Offset >= animation_Comp.animation->get_Frame_Count())
        animation_Comp.animation_Offset = 0;

      animation_Comp.parallax_Factor = 1.0f/(i + 1);
      animation_Comp.offset = {-32, -32, 1.0f/(i + 1)};
      // GEAR_DEBUG_LOG("about to add animation");
      new_Eis->add<gear::AnimationComponent>(animation_Comp);
      // GEAR_DEBUG_LOG("added animation");
      gear::Vector<float, 2> pos(32 - 320 + j * 64.0f, 32 - 180 + i * 64.0f);
      new_Eis->add<gear::TransformComponent>({pos, {1, 1}, 0});
      // GEAR_DEBUG_LOG("added position");
    }

  GEAR_DEBUG_LOG("finished scene");

  cam_Pos = {0, 0};

  gear::Renderer::set_Camera(&cam);
}

void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  //gear::Input::dispatch_Events(nullptr);
  /*
    GEAR_DEBUG_LOG("eis %p", eis);
    auto *pos_Comp = eis->get<gear::TransformComponent>();
    GEAR_DEBUG_LOG("got pos comp %p", pos_Comp);
    auto *pos = &(pos_Comp->data.position);
    GEAR_DEBUG_LOG("got position %p", pos);
    (*pos)[0] -= 2;
    if((*pos)[0] < -64){
      GEAR_DEBUG_LOG("if statement");
      (*pos)[0] = 640;
      palette_Index = (palette_Index + 1) % 3;
      eis->get<gear::SpriteComponent>()->data.palette = palettes[palette_Index];
      GEAR_DEBUG_LOG("set palette");
    }
  */
  // GEAR_DEBUG_LOG("start frame");
  if(gear::Input::get_Key_State(gear::Key::A) == gear::State::PRESSED)
    cam_Pos[0] -= 2;
  
  if(gear::Input::get_Key_State(gear::Key::D) == gear::State::PRESSED)
    cam_Pos[0] += 2;

  if(gear::Input::get_Key_State(gear::Key::W) == gear::State::PRESSED)
    cam_Pos[1] -= 2;

  if(gear::Input::get_Key_State(gear::Key::S) == gear::State::PRESSED)
    cam_Pos[1] += 2;
  

  cam.follow_Target();
  gear::Renderer::start_New_Frame();
  // GEAR_DEBUG_LOG("render scene");
  gear::Renderer::render_Scene(m_Scene);
  // GEAR_DEBUG_LOG("show frame");
  gear::Renderer::show_Frame();

  m_Window->poll_Events();
}

void MyGame::on_Shutdown(void)
{
  GEAR_DEBUG_LOG("destroy scene");
  gear::Renderer::destroy();
  m_Scene->destroy();
  for (int i = 0; i < 7; i++)
    palettes[i] = nullptr;
  GEAR_DEBUG_LOG("unloaded resources: %i", gear::ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}