#include "Scene.h"
#include "Entity.h"

std::vector<gear::Scene> gear::Scene::scenes;

gear::Scene::Scene(void) {}

gear::Scene *gear::Scene::create_Scene(void) {
  scenes.push_back(Scene());
  Scene *scene = &scenes[scenes.size() - 1];
  scene->scene_ID = scenes.size() - 1;
  return scene;
}

void gear::Scene::delete_Scene(uint8_t scene_ID) {
  scenes[scene_ID].destruct_Managers();
  scenes.erase(scenes.begin() + scene_ID);
  Scene* scene = scenes.data();
  for(uint8_t i = scene_ID; i < scenes.size(); i++) {
    (scene + i)->set_ID(i);
  }
}

gear::Scene *gear::Scene::get_Scene(uint8_t scene_ID) {
  return scenes.data() + scene_ID;
}

void gear::Scene::set_ID(uint8_t scene_ID) {
  this->scene_ID = scene_ID;
  for(Entity &entity : entities)
    entity.scene_ID = scene_ID;
}

const uint8_t &gear::Scene::get_ID(void) {
  return scene_ID;
}

gear::Entity *gear::Scene::create_Entity(void) {
  entities.push_back({++next_ID, scene_ID});
  return &entities[entities.size() - 1];
}

gear::Entity *gear::Scene::get_Entity_At(int index) {
  return &entities[index];
}

void gear::Scene::add_Manager_Destructor(std::function<void(void)> function)
{
  manager_Destructors.push_back(function);
}

void gear::Scene::destruct_Managers(void) {
  for(std::function<void(void)> function : manager_Destructors)
    function();
  
  manager_Destructors = std::vector<std::function<void(void)>>(0);
}