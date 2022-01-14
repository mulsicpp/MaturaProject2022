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
  exit(exit_code);
}

void GLAPIENTRY openGL_Debug_Callback(GLenum source,
                                      GLenum type,
                                      GLuint id,
                                      GLenum severity,
                                      GLsizei length,
                                      const GLchar *message,
                                      const void *userParam)
{
  if(type == GL_DEBUG_TYPE_ERROR)
    gear::error("OpenGL error: %s", message);
}

void gear::Game::gear_Init(void)
{
  if (glfwInit() != GLFW_TRUE)
  {
    gear::error("GLFW initialisation failed!\n");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(GEAR_DEBUG)
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  m_Window = Window::create_Window("", 1, 1);
  glfwMakeContextCurrent(m_Window->m_Window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    gear::error("Failed to load OpenGL");


#if defined(GEAR_DEBUG)
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(openGL_Debug_Callback, 0);
#endif


  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void gear::Game::gear_Terminate(void)
{
  glfwMakeContextCurrent(nullptr);
  m_Window->destroy();
  glfwTerminate();
}

const char *gear::Game::get_App_Path(void) const
{
  return m_Path_To_App;
}