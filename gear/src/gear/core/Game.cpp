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

#include <gear/scripting/ScriptComponent.h>
#include <gear/collision/DynamicPhysicsComponent.h>

#include <gear/collision/HitboxComponent.h>

#include <gear/renderer/Renderer.h>

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
    while (!window->should_Close())
    {
        start_Time = glfwGetTime();

        call_Script_Pre_Input();
        poll_Input();
        call_Script_Post_Input();

        call_Script_Pre_Physics();
        physics();
        call_Script_Post_Physics();

        call_Script_Pre_Hitbox_Check();
        hitbox_Check();
        call_Script_Post_Hitbox_Check();

        call_Script_Pre_Render();
        render();
        call_Script_Post_Render();

        m_Delta_Time = glfwGetTime() - start_Time;
    }
    close(0);
}

void gear::Game::on_Startup(void)
{
    allow_Gear_Components();

    window->set_Title("Game Window!");
    window->set_Resizable(false);
    window->set_Size(1280, 720);
    window->set_Visible(true);

    Renderer::create(640, 360);
    Renderer::set_V_Sync(true);

    main_Scene = Scene::get(0);
    main_Scene->create();

    enable_Scripting_For(main_Scene);
}

void gear::Game::on_Shutdown(void)
{
    Renderer::destroy();
    main_Scene->destroy();
}

void gear::Game::poll_Input(void)
{
    Input::dispatch_Events(main_Scene);
}

void gear::Game::physics(void)
{
    physics_Timed_Step(main_Scene);
}

void gear::Game::hitbox_Check(void)
{
    hitbox_Collision_Check(main_Scene);
}

void gear::Game::render(void)
{
    Renderer::start_New_Frame();
    Renderer::render_Scene(main_Scene);
    Renderer::show_Frame();
}

void gear::Game::close(int exit_code)
{
    on_Shutdown();
    gear_Terminate();
    exit(exit_code);
}

void gear::Game::call_Script_Pre_Input(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::pre_Input, scene);
    }
}

void gear::Game::call_Script_Post_Input(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::post_Input, scene);
    }
}

void gear::Game::call_Script_Pre_Physics(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::pre_Physics, scene);
    }
}

void gear::Game::call_Script_Post_Physics(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::post_Physics, scene);
    }
}

void gear::Game::call_Script_Pre_Hitbox_Check(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::pre_Hitbox_Check, scene);
    }
}

void gear::Game::call_Script_Post_Hitbox_Check(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::post_Hitbox_Check, scene);
    }
}

void gear::Game::call_Script_Pre_Render(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::pre_Render, scene);
    }
}

void gear::Game::call_Script_Post_Render(void)
{
    for (auto scene : m_Scripting_Scenes)
    {
        call_Script_Function(&ScriptableEntity::post_Render, scene);
    }
}

void gear::Game::enable_Scripting_For(gear::Scene *scene)
{
    m_Scripting_Scenes.push_back(scene);
}

void gear::Game::disable_Scripting_For(gear::Scene *scene)
{
    for(int i = 0; i < m_Scripting_Scenes.size(); i++)
        if(m_Scripting_Scenes[i] == scene) {
            m_Scripting_Scenes.erase(m_Scripting_Scenes.begin() + i);
        }
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

    window = Window::create_Window("", 1, 1);
    glfwMakeContextCurrent(window->m_Window);
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
    glfwSetScrollCallback(window->m_Window, ScrollEvent::scroll_Event_Callback);
    glfwSetMouseButtonCallback(window->m_Window, MouseButtonEvent::mouse_Button_Event_Callback);
    glfwSetCursorPosCallback(window->m_Window, MouseMovedEvent::mouse_Moved_Event_Callback);
    glfwSetCharCallback(window->m_Window, TextEvent::text_Event_Callback);
    glfwSetKeyCallback(window->m_Window, KeyEvent::key_Event_Callback);
    glfwSetWindowFocusCallback(window->m_Window, WindowFocusEvent::window_Focus_Event_Callback);
    glfwSetWindowIconifyCallback(window->m_Window, WindowIconifyEvent::window_Iconify_Event_Callback);
    glfwSetJoystickCallback(ControllerConnectionEvent::controller_Connection_Event_Callback);
}

void gear::Game::gear_Terminate(void)
{
    gear::Input::destroy();
    glfwMakeContextCurrent(nullptr);
    GEAR_DEBUG_LOG("Destroying window");
    window->destroy();
    glfwTerminate();
}

const char *gear::Game::get_App_Path(void) const { return m_Path_To_App; }

double gear::Game::get_Delta_Time(void) { return m_Delta_Time; }