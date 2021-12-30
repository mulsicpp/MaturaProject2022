#include <gear/gear.h>
#include <stdio.h>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <memory>

#include <gear/data/FileStream.h>
#include <gear/renderer/Renderer.h>
#include <gear/resource/ResourceManager.h>
#include <gear/resource/Palette.h>

#include <filesystem>

void gear::Game::on_Startup(void)
{
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  Window::set_V_Sync(true);
  //  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application ");

  Component<int>::allow();
  gear::component_Flag<int, double>();

  Scene *scene = Scene::get(0);
  scene->create();

  Ref<Font> font = ResourceManager::get<Font>("assets/fonts/font1.gear");
  Ref<Font> font2 = ResourceManager::get<Font>("assets/fonts/font2.gear");
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
  main_Window->destroy();
  GEAR_DEBUG_LOG("%i", ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}