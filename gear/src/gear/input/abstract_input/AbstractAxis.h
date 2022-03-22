#pragma once

#include <gear/core/core.h>
#include <functional>

#define GEAR_AXES(...) \
public:                \
    gear::Ref<gear::AbstractAxis> __VA_ARGS__;

#define GEAR_CREATE_AXIS(type, ...) gear::Ref<gear::AbstractAxis>(new type(__VA_ARGS__))

#define GEAR_CREATE_OR_AXIS(axis1, axis2) gear::Ref<gear::AbstractAxis>(new AbstractOrAxis(axis1, axis2))

_GEAR_START

class AbstractAxis
{
protected:
    std::function<void(float)> m_Callback;

public:
    virtual float get_Value(void) const = 0;
    virtual void set_Callback(const std::function<void(float)> &callback);
};

class AbstractOrAxis : public AbstractAxis
{
protected:
    Ref<AbstractAxis> m_First, m_Second;

public:
    AbstractOrAxis(const Ref<AbstractAxis> &first, const Ref<AbstractAxis> &second);
    float get_Value(void) const override;
};

_GEAR_END