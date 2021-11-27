#pragma once

#include <gear/core/core.h>
#include <vector>
#include <functional>

_GEAR_START

class Entity;

class Scene
{
private:
  uint8_t scene_ID;

  std::vector<Entity> entities;
  unsigned int next_ID = 0;

  std::vector<std::function<void(void)>> manager_Destructors;

  static std::vector<Scene> scenes;

  Scene(void);

public:
  static Scene *create_Scene(void);
  static void delete_Scene(uint8_t scene_ID);
  static Scene *get_Scene(uint8_t scene_ID);

private:
  void set_ID(uint8_t scene_ID);
public:
  const uint8_t &get_ID(void);

  Entity *create_Entity(void);
  //Entity *create_Entity(void(*constructor)(Entity *entity));

  Entity *get_Entity_At(int index);
  Entity *get_Entity_With_ID(int id);

  void add_Manager_Destructor(std::function<void(void)> function);

  void destruct_Managers(void);
};

_GEAR_END