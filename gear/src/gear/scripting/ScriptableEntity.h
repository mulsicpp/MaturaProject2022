#pragma once

#include <gear/core/core.h>
#include <gear/scene/Entity.h>

_GEAR_START

struct ScriptComponent;

class ScriptableEntity
{
protected:
    Entity m_Entity;

public:
    virtual void init(void);
    virtual void pre_Input(void);
    virtual void post_Input(void);
    virtual void pre_Physics(void);
    virtual void post_Physics(void);
    virtual void pre_Hitbox_Check(void);
    virtual void post_Hitbox_Check(void);
    virtual void pre_Render(void);
    virtual void post_Render(void);

    friend class Component<ScriptComponent>;
};

_GEAR_END