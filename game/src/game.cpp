#include <gear/gear.h>
#include <stdio.h>
#include <functional>
#include <map>
#include <vector>
#include <chrono>


void gear::Game::on_Startup(void) {
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);

  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  GEAR_DEBUG_LOG("%s", main_Window);
  main_Window->set_V_Sync(true);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");
  gear::Component_Manager<int>::get_Instance();
  GEAR_DEBUG_LOG("%lli", GEAR_COMP_FLAG(int));
  gear::Component_Manager<float>::get_Instance();
  GEAR_DEBUG_LOG("%lli", GEAR_COMP_FLAG(float));
  gear::Component_Manager<double>::get_Instance();
  GEAR_DEBUG_LOG("%lli", GEAR_COMP_FLAG(double));
}

void gear::Game::per_Frame(void) {
  if(main_Window->should_Close())
    game->close(0);
  main_Window->swap_Buffers();
  main_Window->poll_Events();
}

void gear::Game::on_Shutdown(void) {
  main_Window->destroy();
  GEAR_DEBUG_LOG("Closed application");
  GEAR_DEBUG_LOG_CLOSE_FILE();
}