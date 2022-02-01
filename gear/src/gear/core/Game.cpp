#define GEAR_INCLUDE_WINDOWS_H
#include "Game.h"
#include "debug/log.h"
#include <filesystem>
#include <string.h>
#include "../event/event_Types/ControllerConnectionEvent.h"
#include "../event/event_Types/KeyEvent.h"
#include "../event/event_Types/MouseButtonEvent.h"
#include "../event/event_Types/MouseMovedEvent.h"
#include "../event/event_Types/ScrollEvent.h"
#include "../event/event_Types/TextEvent.h"
#include "../event/event_Types/WindowFocusEvent.h"
#include "../event/event_Types/WindowIconifyEvent.h"
#include "../event/Input.h"

#if defined(GEAR_PLATFORM_LINUX)
#include <unistd.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

gear::Game::Game(void)
{
#if defined(GEAR_PLATFORM_WINDOWS)
  char temp_Path[MAX_PATH];
  GetModuleFileNameA(NULL, temp_Path, MAX_PATH);
  std::filesystem::path path = std::filesystem::weakly_canonical(std::filesystem::path(temp_Path).parent_path());
  strcpy((char *)m_Path_To_App, path.string().c_str());

#elif defined(GEAR_PLATFORM_LINUX)
  char temp_Path[4092];
  readlink("/proc/self/exe", temp_Path, 4092);
  std::filesystem::path path = std::filesystem::weakly_canonical(std::filesystem::path(temp_Path).parent_path());
  strcpy((char *)m_Path_To_App, path.string().c_str());
#endif
}

void gear::Game::run(void)
{
  std::filesystem::current_path(m_Path_To_App);
  std::filesystem::current_path(GEAR_ROOT_PATH);
  gear_Init();
  on_Startup();
  while (1)
  {
    per_Frame();
  }
}

void gear::Game::close(int exit_code)
{
  on_Shutdown();
  gear_Terminate();
  exit(exit_code);
}

void APIENTRY openGL_Debug_Callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
  gear::error("An OpenGL error has occured: %s\nseverity: %u\ntype:%u\nsource:%u", message, severity, type, source);
}

void gear::Game::gear_Init(void)
{
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
  if (glfwInit() != GLFW_TRUE)
  {
    gear::error("GLFW initialisation failed!\n");
  }
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  m_Window = Window::create_Window("", 1, 1);
  glfwMakeContextCurrent(m_Window->m_Window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    gear::error("Failed to load OpenGL");

#if defined(GEAR_DEBUG)
  glDebugMessageCallback(openGL_Debug_Callback, nullptr);
#endif

  gear::Input::init();
  glfwSetScrollCallback(m_Window->m_Window, ScrollEvent::scroll_Event_Callback);
  glfwSetMouseButtonCallback(m_Window->m_Window, MouseButtonEvent::mouse_Button_Event_Callback);
  glfwSetCursorPosCallback(m_Window->m_Window, MouseMovedEvent::mouse_Moved_Event_Callback);
  glfwSetCharCallback(m_Window->m_Window, TextEvent::text_Event_Callback);
  glfwSetKeyCallback(m_Window->m_Window, KeyEvent::key_Event_Callback);
  glfwSetWindowFocusCallback(m_Window->m_Window, WindowFocusEvent::window_Focus_Event_Callback);
  glfwSetWindowIconifyCallback(m_Window->m_Window, WindowIconifyEvent::window_Iconify_Event_Callback);
  glfwSetJoystickCallback(ControllerConnectionEvent::controller_Connection_Event_Callback);
}

void gear::Game::gear_Terminate(void)
{
  m_Window->destroy();
  glfwTerminate();
}