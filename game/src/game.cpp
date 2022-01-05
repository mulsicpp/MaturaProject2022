#include <gear/gear.h>
#include <stdio.h>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <memory>
#include <filesystem>

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>
#include <gear/renderer/Renderer.h>
#include <gear/renderer/SpriteComponent.h>
#include <gear/resource/Palette.h>
#include <gear/resource/Sprite.h>
#include <gear/scene/PositionComponent.h>

class A {
public:
  A();
  A(int data[4]);
};

void gear::Game::on_Startup(void)
{
  //  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  Window::set_V_Sync(true);
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  main_Window->set_Resizable(false);
  main_Window->make_Renderable(640, 360);

  Renderer::set_Window(main_Window);

  Component<PositionComponent>::allow();
  Component<SpriteComponent>::allow();

  Scene *scene = Scene::get(0);
  scene->create();

  GEAR_DEBUG_LOG("Creating entity");

  Entity *entity = scene->create_Entity();

  GEAR_DEBUG_LOG("Adding component");
  SpriteComponent sprite_Component;
  sprite_Component.offset = { 0, 0 };
  sprite_Component.sprite = ResourceManager::get<Sprite>("assets/test_sprites/kirby_cook.gear");
  sprite_Component.palette = ResourceManager::get<Palette>("assets/test_sprites/kirby_cook_palette.gear");

  entity->add<SpriteComponent>(sprite_Component);
  entity->add<PositionComponent>({{0, 0, 0}});

  GEAR_DEBUG_LOG("Finished entity");
}

void gear::Game::per_Frame(void)
{
  if (main_Window->should_Close())
    game->close(0);

  main_Window->swap_Buffers();
  main_Window->poll_Events();
}

void gear::Game::on_Shutdown(void)
{
  GEAR_DEBUG_LOG("Destroying scene");
  Scene::get(0)->destroy();
  GEAR_DEBUG_LOG("unloaded resources: %i", ResourceManager::unload());
  main_Window->destroy();
  GEAR_DEBUG_LOG("%i", ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}