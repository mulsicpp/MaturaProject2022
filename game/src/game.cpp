#include <gear/gear.h>
#include <stdio.h>
#include <functional>
#include <map>
#include <vector>
#include <chrono>
#include <memory>

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>

gear::Ref<gear::Font> font;

#include <filesystem>

void gear::Game::on_Startup(void)
{
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  Window::set_V_Sync(true);
  //  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");
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
  GEAR_DEBUG_LOG("unloaded resources: %i", ResourceManager::unload());
  main_Window->destroy();
  GEAR_DEBUG_LOG("%i", ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}