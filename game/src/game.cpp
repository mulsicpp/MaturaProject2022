#include <gear/gear.h>
#include <stdio.h>
#include <functional>
#include <map>
#include <vector>
#include <chrono>

struct X {
  int a;
  long long int b;
  double c;
};

void double_Entity(gear::Entity *entity) {
  entity->add<double>(gear::PI);
}

void gear::Game::on_Startup(void)
{
  main_Window = gear::Window::create_Window("Game Window", 1280, 720);
  Window::set_V_Sync(true);
//  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG_OPEN_FILE("log.txt");
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_FILE | GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  Component<int>::allow();
  Component<double>::allow();
  Component<float>::allow();
  Component<short>::allow();
  Component<char>::allow();

  Scene &scene = *Scene::get(0);
  scene.create();

  for(int i = 0; i < 10; i++){
    Entity *entity = scene.create_Entity();
    if(i % 2 == 0)
      entity->add<int>(i);
    if(i / 3 == 1)
      entity->add<double>(i);
    if(i % 4 != 0)
      entity->add<short>(i);
    if(i == 3 || i == 5 || i == 8)
      entity->add<char>('a' + i);
  }
  scene.print();

  scene.get_Entity_With_ID(1)->add<double>(1);
  scene.get_Entity_With_ID(1)->add<double>(2);
  scene.get_Entity_With_ID(4)->set<double>(7);
  scene.get_Entity_With_ID(5)->set<double>(7);
  scene.get_Entity_With_ID(8)->set<double>(7);
  scene.print();

  scene.destroy();
  scene.print();
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