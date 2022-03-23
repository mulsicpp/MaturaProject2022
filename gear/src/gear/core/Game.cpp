#define GEAR_INCLUDE_WINDOWS_H
#include "Game.h"
#include "debug/log.h"
#include <filesystem>
#include <string.h>
#include <gear/renderer/Renderer.h>
#include "../input/events/ControllerConnectionEvent.h"
#include "../input/events/KeyEvent.h"
#include "../input/events/MouseButtonEvent.h"
#include "../input/events/MouseMovedEvent.h"
#include "../input/events/ScrollEvent.h"
#include "../input/events/TextEvent.h"
#include "../input/events/WindowFocusEvent.h"
#include "../input/events/WindowIconifyEvent.h"
#include "../input/Input.h"

#if defined(GEAR_PLATFORM_LINUX)
#include <unistd.h>
#endif

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

double gear::Game::m_Delta_Time = 0;

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
    double start_Time = 0;
    while (1)
    {
        start_Time = glfwGetTime();
        per_Frame();
        m_Delta_Time = glfwGetTime() - start_Time;
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
    if (type == GL_DEBUG_TYPE_ERROR)
        gear::error("OpenGL error: %s", message);
}

void gear::Game::gear_Init(void)
{
    GEAR_DEBUG_LOG_SET_OUTPUT(GEAR_CONSOLE);
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
    gear::Input::destroy();
    glfwMakeContextCurrent(nullptr);
    GEAR_DEBUG_LOG("Destroying window");
    m_Window->destroy();
    glfwTerminate();
}

const char *gear::Game::get_App_Path(void) const { return m_Path_To_App; }

double gear::Game::get_Delta_Time(void) { return m_Delta_Time; }