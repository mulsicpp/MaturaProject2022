#pragma once

#include <gear/core/core.h>
#include <gear/scene/Entity.h>

_GEAR_START

struct ScriptComponent;

class ScriptableEntity {
protected:
  Entity m_Entity;

public:
  virtual void on_Create(void);
  virtual void on_Update(void);
  virtual void on_Destroy(void);

  friend class Component<ScriptComponent>;
};

_GEAR_END