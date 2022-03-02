#pragma once

#include <gear/core/core.h>
#include "InputSelector.h"

_GEAR_START

class AbstractKeyboardInput
{
public:
  AbstractKeyboardInput(void);
};

class AbstractControllerInput
{
protected:
  int m_ID;

public:
  AbstractControllerInput(int id);
  int get_ID();
};

_GEAR_END

#define GEAR_ABSTRACT_INPUT(name, keyboard, controller, body)          \
  class name;                                                          \
  class controller;                                                    \
  class keyboard;                                                      \
  class name : public gear::InputSelector<name, keyboard, controller>  \
  {                                                                    \
    body                                                               \
  };                                                                   \
  class keyboard : public name, public gear::AbstractKeyboardInput     \
  {                                                                    \
  public:                                                              \
    keyboard(void);                                                    \
  };                                                                   \
  class controller : public name, public gear::AbstractControllerInput \
  {                                                                    \
  public:                                                              \
    controller(int id);                                                \
  };                                                                   