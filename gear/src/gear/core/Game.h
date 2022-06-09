#pragma once

#include "core.h"
#include "Window.h"

#include <gear/scene/Scene.h>
#include <vector>

#if defined(GEAR_DEBUG)
#define GEAR_ROOT_PATH "../../../.."
#endif

#if !defined(GEAR_ROOT_PATH)
#define GEAR_ROOT_PATH "."
#endif

_GEAR_START

class Game
{
private:
    const char m_Path_To_App[512]{0};
    static double m_Delta_Time;
    std::vector<Scene *> m_Scripting_Scenes;

    void gear_Init(void);

    void gear_Terminate(void);

    void call_Script_Pre_Input(void);
    void call_Script_Post_Input(void);
    void call_Script_Pre_Physics(void);
    void call_Script_Post_Physics(void);
    void call_Script_Pre_Hitbox_Check(void);
    void call_Script_Post_Hitbox_Check(void);
    void call_Script_Pre_Render(void);
    void call_Script_Post_Render(void);

protected:
    gear::Window *window = nullptr;
    Scene *main_Scene;

public:
    Game(void);
    void run(void);

    virtual void on_Startup(void);
    virtual void on_Shutdown(void);

    virtual void poll_Input(void);
    virtual void physics(void);
    virtual void hitbox_Check(void);
    virtual void render(void);

    void close(int exit_code);

    void enable_Scripting_For(Scene *scene);
    void disable_Scripting_For(Scene *scene);

    const char *get_App_Path(void) const;

    static double get_Time(void);
    static double get_Delta_Time(void);
};

_GEAR_END