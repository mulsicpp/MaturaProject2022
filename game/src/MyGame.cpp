#include "MyGame.h"

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>
#include <gear/renderer/Renderer.h>
#include <gear/renderer/SpriteComponent.h>

#include <gear/scene/PositionComponent.h>

void MyGame::on_Startup(void)
{
  m_Window->set_Title("Game Window!");
  m_Window->set_Resizable(false);
  m_Window->set_Size(1280, 720);
  //m_Window->set_Fullscreen();
  m_Window->set_Visible(true);

  gear::Renderer::create(320, 180);
  gear::Renderer::set_V_Sync(true);
  
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  gear::allow_Gear_Components();

  m_Scene = gear::Scene::get(0);
  m_Scene->create();

  palettes[0] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_yellow.gear");
  palettes[1] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_pink.gear");
  palettes[2] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_green.gear");

  eis = m_Scene->create_Entity();

  gear::SpriteComponent sprite_Comp;
  sprite_Comp.offset = {0, 0};
  sprite_Comp.parallax_Factor = 1;
  //sprite_Comp.sprite = gear::ResourceManager::get<gear::Sprite>("assets/test_sprites/triangle.gear");
  //sprite_Comp.palette = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/triangle_palette.gear");
  sprite_Comp.sprite = gear::ResourceManager::get<gear::Sprite>("assets/test_sprites/eis.gear");
  sprite_Comp.palette = palettes[0];

  eis->add<gear::SpriteComponent>(sprite_Comp);
  eis->add<gear::PositionComponent>({{0, 0, 0}});
  /*
  for(int i = 0; i < 20; i++)
    for(int j = 0; j < 5; j++)
    {
      gear::Entity* new_Eis = m_Scene->create_Entity();
      sprite_Comp.palette = palettes[(i + j) % 3];
      new_Eis->add<gear::SpriteComponent>(sprite_Comp);
      gear::Vector<float, 3> pos(i * 50.0f, 60.0f + j * 60.0f, 0.0f);
      new_Eis->add<gear::PositionComponent>({pos});
      GEAR_DEBUG_LOG("added eis %i %i", i, j);
    }
  */

  eis_Idle = m_Scene->create_Entity();

  gear::AnimationComponent animation_Comp;
  animation_Comp.offset = {0, 0};
  animation_Comp.parallax_Factor = 1;
  animation_Comp.animation = gear::ResourceManager::get<gear::Animation>("assets/test_sprites/eis_idle.gear");
  animation_Comp.animation_Offset = 1;
  animation_Comp.frame_Rate = animation_Comp.animation->get_Default_Frame_Rate();
  animation_Comp.palette = palettes[0];

  eis_Idle->add<gear::AnimationComponent>(animation_Comp);
  eis_Idle->add<gear::PositionComponent>({{0, 70, 0}});
}

void MyGame::per_Frame(void)
{
  GEAR_DEBUG_LOG("entered loop");
  if (m_Window->should_Close())
    this->close(0);
  GEAR_DEBUG_LOG("window still open");
  auto &pos = eis->get<gear::PositionComponent>()->data.position;
  pos[0] -= 2;
  if(pos[0] < -64){
    pos[0] =640;
    palette_Index = (palette_Index + 1) % 3;
    eis->get<gear::SpriteComponent>()->data.palette = palettes[palette_Index];
  }
  
  GEAR_DEBUG_LOG("script");
  gear::Renderer::start_New_Frame();
  GEAR_DEBUG_LOG("start frame");
  gear::Renderer::render_Scene(m_Scene);
  GEAR_DEBUG_LOG("render scene");
  gear::Renderer::show_Frame();
  GEAR_DEBUG_LOG("show frame");

  m_Window->poll_Events();
  GEAR_DEBUG_LOG("poll events");


}

void MyGame::on_Shutdown(void)
{
  m_Scene->destroy();
  GEAR_DEBUG_LOG("unloaded resources: %i", gear::ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}