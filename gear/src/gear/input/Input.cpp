#include "Input.h"
#include "events/Event.h"
#include "EventQueue.h"
#include <chrono>
#include <thread>
#include "events/MouseMovedEvent.h"
#include "events/ScrollEvent.h"
#include "events/TextEvent.h"
#include "events/WindowFocusEvent.h"
#include "events/WindowIconifyEvent.h"

#include "gamepad_mappings.h"

GLFWwindow *gear::Input::m_Window = nullptr;
bool gear::Input::m_Controller_Thread_Running = false;
std::thread gear::Input::m_Controller_Thread;
std::unordered_map<int, GLFWgamepadstate> gear::Input::m_Controller_Map;

void gear::Input::controller_Loop(void)
{
  GLFWgamepadstate prev_State;
  while (m_Controller_Thread_Running)
  {
    for (auto &[key, value] : m_Controller_Map)
    {
      prev_State = value;
      glfwGetGamepadState(key, &value);
      for (int i = 0; i < 15; i++)
      {
        if (value.buttons[i] != prev_State.buttons[i])
          gear::EventQueue<ControllerButtonEvent>::push_Event({key, i, value.buttons[i]});
      }
      for (int i = 0; i < 6; i++)
      {
        if (value.axes[i] != prev_State.axes[i])
          gear::EventQueue<ControllerAxisEvent>::push_Event({key, i, value.axes[i]});
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
  }
}

void gear::Input::update_Controllermap(void)
{
  m_Controller_Map.clear();
  for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++)
  {
    if (glfwJoystickPresent(i) && glfwJoystickIsGamepad(i))
    {
      glfwGetGamepadState(i, &m_Controller_Map[i]);
    }
  }
}

void gear::Input::init(void)
{
  m_Window = glfwGetCurrentContext();

  for (const char *mapping : gamepad_Mappings)
    glfwUpdateGamepadMappings(mapping);

  update_Controllermap();
  add_Global_Callback<ControllerConnectionEvent>([](ControllerConnectionEvent event)
                                                 { update_Controllermap(); });
  m_Controller_Thread_Running = true;
  m_Controller_Thread = std::thread(controller_Loop);
}

gear::State gear::Input::get_Key_State(Key key)
{
  int state = glfwGetKey(m_Window, (int)key);
  return (State)state;
}

gear::Vector<double, 2> gear::Input::get_Cursor_Position()
{
  double xpos, ypos;
  glfwGetCursorPos(m_Window, &xpos, &ypos);
  return {xpos, ypos};
}

gear::State gear::Input::get_Mouse_Button_State(MouseButton mouse_Button)
{
  int state = glfwGetMouseButton(m_Window, (int)mouse_Button);
  return (State)state;
}
float gear::Input::get_Axis_Value(int controller_Id, ControllerAxis axis)
{
  GLFWgamepadstate state;
  if(glfwGetGamepadState(controller_Id, &state)) {
    return state.axes[(int)axis];
  }
  return 0;
}

gear::State gear::Input::get_Controller_Button_State(int controller_Id, ControllerButton controller_Button)
{
  GLFWgamepadstate state;
  if(glfwGetGamepadState(controller_Id, &state)) {
    return state.buttons[(int)controller_Button] ? State::PRESSED : State::RELEASED;
  }
  return State::RELEASED;
}

void gear::Input::dispatch_Events(Scene *scene)
{
  glfwPollEvents();
  EventQueue<ControllerAxisEvent>::dispatch_Events(scene);
  EventQueue<ControllerButtonEvent>::dispatch_Events(scene);
  EventQueue<ControllerConnectionEvent>::dispatch_Events(scene);
  EventQueue<KeyEvent>::dispatch_Events(scene);
  EventQueue<MouseButtonEvent>::dispatch_Events(scene);
  EventQueue<MouseMovedEvent>::dispatch_Events(scene);
  EventQueue<ScrollEvent>::dispatch_Events(scene);
  EventQueue<TextEvent>::dispatch_Events(scene);
  EventQueue<WindowFocusEvent>::dispatch_Events(scene);
  EventQueue<WindowIconifyEvent>::dispatch_Events(scene);
}

void gear::Input::destroy(void)
{
  m_Controller_Thread_Running = false;
  m_Controller_Thread.join();
}
