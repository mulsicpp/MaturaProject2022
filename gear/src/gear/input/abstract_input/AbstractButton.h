#pragma once

#include <gear/core/core.h>
#include "../Input.h"

#include "AbstractElement.h"

#include <vector>

#define GEAR_BUTTONS(...) \
public:                   \
    gear::Ref<gear::AbstractButton> __VA_ARGS__;

#define GEAR_CREATE_BUTTON(type, ...) gear::Ref<gear::AbstractButton>(new type(__VA_ARGS__))

#define GEAR_CREATE_OR_BUTTON(...) gear::Ref<gear::AbstractButton>(new gear::AbstractOrButton({__VA_ARGS__}));

_GEAR_START

class AbstractButton : public AbstractElement<gear::Action>
{
public:
    virtual State get_State(void) const = 0;
    virtual void set_Enabled(bool enabled) override;
};

class AbstractOrButton : public AbstractButton
{
protected:
    std::vector<Ref<AbstractButton>> m_Buttons;

public:
    AbstractOrButton(const std::vector<Ref<AbstractButton>> &buttons);
    State get_State(void) const override;
    void set_Callback(const std::function<void(Action)> &callback) override;
    virtual void set_Enabled(bool enabled) override;
};

_GEAR_END