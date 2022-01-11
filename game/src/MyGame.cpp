#include "MyGame.h"

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>
#include <gear/renderer/Renderer.h>
#include <gear/renderer/SpriteComponent.h>

#include <gear/scene/PositionComponent.h>

void MyGame::on_Startup(void)
{
  m_Window->set_Size(1280, 720);
  m_Window->set_Title("Game Window!");
  m_Window->set_Resizable(false);
  m_Window->set_Visible(true);

  gear::Renderer::create(320, 180);
  gear::Renderer::set_V_Sync(true);
  gear::Renderer::setup_Test_Frame();
  
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  gear::allow_Gear_Components();

  m_Scene = gear::Scene::get(0);
  m_Scene->create();

  eis = m_Scene->create_Entity();
  palettes[0] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_yellow.gear");
  palettes[1] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_pink.gear");
  palettes[2] = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/eis_palette_green.gear");

  gear::SpriteComponent sprite_Comp;
  sprite_Comp.offset = {0, 0};
  sprite_Comp.parallax_Factor = 1;
  //sprite_Comp.sprite = gear::ResourceManager::get<gear::Sprite>("assets/test_sprites/triangle.gear");
  //sprite_Comp.palette = gear::ResourceManager::get<gear::Palette>("assets/test_sprites/triangle_palette.gear");
  sprite_Comp.sprite = gear::ResourceManager::get<gear::Sprite>("assets/test_sprites/eis.gear");
  sprite_Comp.palette = palettes[0];

  eis->add<gear::SpriteComponent>(sprite_Comp);
  eis->add<gear::PositionComponent>({{0, 0, 0}});

}

void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  auto &pos = eis->get<gear::PositionComponent>()->data.position;
  pos[0] -= 1.5;
  if(pos[0] < -64){
    pos[0] = 320;
    palette_Index = (palette_Index + 1) % 3;
    eis->get<gear::SpriteComponent>()->data.palette = palettes[palette_Index];
  }
  gear::Renderer::start_New_Frame();
  gear::Renderer::render_Scene(m_Scene);
  gear::Renderer::show_Frame();

  m_Window->poll_Events();
}

void MyGame::on_Shutdown(void)
{
  m_Scene->destroy();
  GEAR_DEBUG_LOG("unloaded resources: %i", gear::ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}