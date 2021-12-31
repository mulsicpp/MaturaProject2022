#define GEAR_INCLUDE_WINDOWS_H
#include "Game.h"
#include "debug/log.h"
#include <filesystem>
#include <string.h>
#include <gear/renderer/Renderer.h>

#if defined(GEAR_PLATFORM_LINUX)
#include <unistd.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

gear::Game *gear::Game::game = nullptr;

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

gear::Game *gear::Game::get_Instance(void)
{
  if (game == nullptr)
    game = new Game();
  return game;
}

void gear::Game::run(void)
{
  GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
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
  delete this;
  exit(exit_code);
}

void APIENTRY openGL_Debug_Callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
  gear::error("An OpenGL error has occured: %s\nseverity: %u\ntype:%u\nsource:%u", message, severity, type, source);
}

void gear::Game::gear_Init(void)
{
  if (glfwInit() != GLFW_TRUE)
  {
    GEAR_DEBUG_LOG("GLFW initialisation failed!\n");
    gear::Game::get_Instance()->close(1);
  }

  Renderer::create();

#if defined(GEAR_DEBUG)
  glDebugMessageCallback(openGL_Debug_Callback, nullptr);
#endif
}

void gear::Game::gear_Terminate(void)
{
  Renderer::destroy();
  glfwTerminate();
}

const char *gear::Game::get_App_Path(void) const
{
  return m_Path_To_App;
}