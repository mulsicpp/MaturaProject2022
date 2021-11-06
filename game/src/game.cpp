#include <gear/gear.h>
#include <gear/scene/Entity.h>
#include <gear/math/math.h>
#include <stdio.h>
#include <functional>
#include <map>
#include <vector>
#include <chrono>


void gear::Game::on_Startup(void) {
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  main_Window->set_V_Sync(true);
}

void gear::Game::per_Frame(void) {
  if(main_Window->should_Close())
    game->close(0);
  main_Window->swap_Buffers();
  main_Window->poll_Events();
}

void gear::Game::on_Shutdown(void) {
  main_Window->destroy();
  GEAR_DEBUG_LOG_CLOSE_FILE();
}