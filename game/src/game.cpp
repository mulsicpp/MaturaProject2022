#include <gear/gear.h>
#include <stdio.h>

void gear::Game::on_Startup(void) {
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  main_Window->set_V_Sync(true);
  GEAR_DEBUG_LOG_OPEN_FILE("debug_log.txt");
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE | GEAR_FILE);
  GEAR_DEBUG_LOG("hallo welt1");
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_FILE);
  GEAR_DEBUG_LOG("hallo welt2");
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("hallo welt3");
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