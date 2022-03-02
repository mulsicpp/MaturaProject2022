#include "MyGame.h"

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>

#include <gear/input/events/KeyEvent.h>
#include <gear/input/InputComponent.h>
#include <gear/input/Input.h>

#include <gear/input/abstract_input/abstract_input.h>

using namespace gear;

GEAR_ABSTRACT_INPUT(UIInput, UIKeyboardInput, UIControllerInput, public: int x; );

UIKeyboardInput::UIKeyboardInput() {

}

UIControllerInput::UIControllerInput(int id) : gear::AbstractControllerInput(id) {

}

void MyGame::on_Startup(void)
{
  Ref<UIInput> input = UIInput::create_From(InputDevice::CONTROLLER_10);
  input->x = 0;
  m_Window->set_Size(1280, 720);
  m_Window->set_Title("AHHHHHHH!");
  m_Window->set_Visible(true);
  //  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  GEAR_DEBUG_LOG("%i", ((UIControllerInput *)input.get())->get_ID());

  gear::InputComponent<gear::KeyEvent> e = {[](gear::KeyEvent e){}, false};
  gear::Input::add_Global_Callback<gear::ControllerButtonEvent>([](gear::ControllerButtonEvent e){GEAR_DEBUG_LOG("gamepad button %s", e.get_Action() == gear::Action::PRESSED ? "pressed" : "released");});
}

void MyGame::per_Frame(void)
{
  if (m_Window->should_Close())
    this->close(0);
  gear::Input::dispatch_Events(nullptr);
  m_Window->swap_Buffers();
  m_Window->poll_Events();
}

void MyGame::on_Shutdown(void)
{
  //GEAR_DEBUG_LOG("unloaded resources: %i", gear::ResourceManager::unload());
  GEAR_DEBUG_LOG("Closed application");
}