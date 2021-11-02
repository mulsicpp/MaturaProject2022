#include <gear/gear.h>
#include <stdio.h>

void gear::Game::on_Startup(void) {
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
}

void gear::Game::per_Frame(void) {
  if(main_Window->should_Close())
    game->close(0);
  main_Window->swap_Buffers();
  main_Window->poll_Events();
}

void gear::Game::on_Shutdown(void) {
  main_Window->destroy();
}