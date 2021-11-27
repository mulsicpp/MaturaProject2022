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
  gear::Component<int> ci;
  GEAR_DEBUG_LOG("%lli", gear::Component<int>::get_Flag());
  gear::Component<float> cf;
  GEAR_DEBUG_LOG("%lli", gear::Component<float>::get_Flag());
  gear::Component<double> cd;
  GEAR_DEBUG_LOG("%lli", gear::Component<double>::get_Flag());
  GEAR_DEBUG_LOG("%lli", gear::component_Flag<int, double, float>());

  Scene *scene = Scene::create_Scene();
  for(int i = 0; i < 6; i++) {
    Entity *entity = scene->create_Entity();
    if(i % 3 == 0)
      entity->add_Component<int>(i);
    if(i % 2 == 0)
      entity->add_Component<float>(i);
    if(i % 4 != 0)
      entity->add_Component<double>(i);

    GEAR_DEBUG_LOG("%i %i %i", entity->has_Component<int>(), entity->has_Component<float>(), entity->has_Component<double>());
    Component<int> *i_Comp = entity->get_Component<int>();
    Component<float> *f_Comp = entity->get_Component<float>();
    Component<double> *d_Comp = entity->get_Component<double>();
    GEAR_DEBUG_LOG("%i %f %lf", i_Comp ? i_Comp->data : -1, f_Comp ? f_Comp->data : -1, d_Comp ? d_Comp->data : -1);
  }
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