#include "MyGame.h"

#include <gear/data/FileStream.h>
#include <gear/resource/ResourceManager.h>

#include <gear/input/events/KeyEvent.h>
#include <gear/input/InputComponent.h>
#include <gear/input/Input.h>

#include <gear/input/abstract_input/elements/AMouseButton.h>
#include <gear/input/abstract_input/elements/AKeyboardButton.h>
#include <gear/input/abstract_input/elements/AControllerButton.h>

using namespace gear;

UIKeyboardInput::UIKeyboardInput() {
  submit = GEAR_CREATE_OR_BUTTON(GEAR_CREATE_BUTTON(AMouseButton, MouseButton::LEFT), GEAR_CREATE_BUTTON(AKeyboardButton, Key::ENTER));
  back = GEAR_CREATE_BUTTON(AMouseButton, MouseButton::RIGHT);
}

UIControllerInput::UIControllerInput(int id) : gear::AbstractControllerInput(id) {
  submit = GEAR_CREATE_BUTTON(AControllerButton, m_ID, ControllerButton::B);
  back = GEAR_CREATE_BUTTON(AControllerButton, m_ID, ControllerButton::A);
}

void MyGame::on_Startup(void)
{
  input1 = UIInput::create_From(InputDevice::KEYBOARD);

  input1->submit->set_Callback([](gear::Action action) {
    GEAR_DEBUG_LOG("submit 1");
  });

  input1->back->set_Callback([](gear::Action action) {
    GEAR_DEBUG_LOG("back 1");
  });

  input2 = UIInput::create_From(InputDevice::CONTROLLER_1);

  input2->submit->set_Callback([](gear::Action action) {
    GEAR_DEBUG_LOG("submit 2");
  });

  input2->back->set_Callback([](gear::Action action) {
    GEAR_DEBUG_LOG("back 2");
  });

  m_Window->set_Size(1280, 720);
  m_Window->set_Title("AHHHHHHH!");
  m_Window->set_Visible(true);
  //  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  GEAR_DEBUG_LOG("Opened application");

  gear::InputComponent<gear::KeyEvent> e = {[](gear::KeyEvent e){}, false};
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