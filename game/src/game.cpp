#include <gear/gear.h>
#include <stdio.h>
#include <functional>
#include <map>
#include <vector>
#include <chrono>

void gear::Game::on_Startup(void)
{
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  main_Window->set_V_Sync(true);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  std::chrono::time_point start = std::chrono::steady_clock::now();

  WeakVector<double, 1> v;
  v.create();
  for(int i = 0; i < 10000; i++) {
    v.push_Back(i);
  }

  for(int i = 0; i < 10000; i++) {
    v.pop_Back();
  }
  v.destroy();

  std::chrono::time_point end = std::chrono::steady_clock::now();

  GEAR_DEBUG_LOG("%lli ns", (end - start).count());

  start = std::chrono::steady_clock::now();

  WeakVector<double, 12> v2;
  v2.create();
  for(int i = 0; i < 10000; i++) {
    v2.push_Back(i);
  }

  for(int i = 0; i < 10000; i++) {
    v2.pop_Back();
  }
  v2.destroy();

  end = std::chrono::steady_clock::now();

  GEAR_DEBUG_LOG("%lli ns", (end - start).count());
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
  GEAR_DEBUG_LOG("Closed application");
  GEAR_DEBUG_LOG_CLOSE_FILE();
}