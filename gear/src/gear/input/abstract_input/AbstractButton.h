#pragma once

#include <gear/core/core.h>
#include "../Input.h"

#define GEAR_BUTTONS(...) public: gear::Ref<gear::AbstractButton> __VA_ARGS__;

#define GEAR_CREATE_BUTTON(type, ...) gear::Ref<gear::AbstractButton>(new type(__VA_ARGS__))

#define GEAR_CREATE_OR_BUTTON(button1, button2) gear::Ref<gear::AbstractButton>(new gear::AbstractOrButton(button1, button2));

_GEAR_START

class AbstractButton {
protected:
  std::function<void(Action)> m_Callback;
public:
  virtual State get_State(void) const = 0;
  virtual void set_Callback(const std::function<void(Action)> &callback);
};

class AbstractOrButton : public AbstractButton {
protected:
    Ref<AbstractButton> m_First, m_Second;
public:
    AbstractOrButton(const Ref<AbstractButton>& first, const Ref<AbstractButton>& second);
    State get_State(void) const override;
    void set_Callback(const std::function<void(Action)> &callback) override;
};

_GEAR_END