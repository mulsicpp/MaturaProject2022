#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>
#include <gear/scene/Entity.h>


_GEAR_START

class UIComponent
{
private:
    Entity m_Entity;
    UIComponent(Entity entity);
    UIComponent(const UIComponent &container) = delete;
    UIComponent &operator=(const UIComponent &container) = delete;

public:
    virtual bool is_Focusable(void) const;

    void set_Position(Vector<double, 2> position);
    Vector<double, 2> get_Position(void) const;
};

_GEAR_END