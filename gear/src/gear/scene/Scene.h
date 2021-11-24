#pragma once

#include <gear/core/core.h>
#include "Entity.h"
#include <vector>

_GEAR_START

class Scene
{
private:
  int index = -1;
  const char *name;
  static std::vector<Scene> scenes;

public:
  static Scene *create_Scene(const char *name);

  static Scene *delete_Scene(const char *name);
  static Scene *delete_Scene(int index);

  static Scene *get_Scene(int index);
  static Scene *get_Scene(const char *name);

  Entity *create_Entity(void);
  Entity *create_Entity(void(*constructor)(Entity *entity));

  Entity *get_Entity_At(int index);
  Entity *get_Entity_With_ID(int id);
};

_GEAR_END