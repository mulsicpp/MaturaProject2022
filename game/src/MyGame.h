#include <gear/gear.h>

#include <gear/input/abstract_input/abstract_input.h>

#include <gear/input/abstract_input/AbstractButton.h>
#include <gear/input/abstract_input/AbstractAxis.h>
#include <gear/input/abstract_input/AbstractDirection.h>

GEAR_ABSTRACT_INPUT(UIInput, UIKeyboardInput, UIControllerInput, GEAR_BUTTONS(submit, back));

class MyGame : public gear::Game
{
  gear::Ref<UIInput> input1, input2;
  void on_Startup(void) override;
  void per_Frame(void) override;
  void on_Shutdown(void) override;
};