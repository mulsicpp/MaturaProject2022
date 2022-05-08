#include "Renderer.h"
#include <glad/glad.h>
#include <gear/core/debug/log.h>
#include <gear/data/FileStream.h>

#include <gear/scene/Entity.h>
#include "animation_player.h"

#include "pipelines/UpscalePipeline.h"
#include "pipelines/SpritePipeline.h"
#include "pipelines/ShapePipeline.h"
#include "pipelines/TextPipeline.h"

#include <gear/collision/DynamicPhysicsComponent.h>
#include <gear/collision/HitboxComponent.h>
#include <gear/collision/HurtboxComponent.h>

#include <gear/ui/UI.h>

using SpritePipeline = gear::SpritePipeline;

gear::Framebuffer gear::Renderer::m_Framebuffer;

gear::Camera *gear::Renderer::m_Camera = nullptr;

GLFWwindow *gear::Renderer::m_Window = nullptr;

gear::Vector<int, 2> gear::Renderer::m_Top_Left;
gear::Vector<int, 2> gear::Renderer::m_Bottom_Right;
gear::Vector<float, 4> gear::Renderer::m_Clear_Color;

void gear::Renderer::create(int width, int height)
{
    m_Window = glfwGetCurrentContext();
    int window_Width, window_Height;
    glfwGetWindowSize(m_Window, &window_Width, &window_Height);
    glViewport(0, 0, window_Width, window_Height);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GEQUAL);
    glClearDepth(-1.0);

    m_Framebuffer.create(width, height);

    UpscalePipeline::get_Instance().init();
    SpritePipeline::get_Instance().init();
    ShapePipeline::get_Instance().init();
    TextPipeline::get_Instance().init();
}

void gear::Renderer::destroy(void)
{
    m_Framebuffer.destroy();

    UpscalePipeline::get_Instance().destroy();
    SpritePipeline::get_Instance().destroy();
    ShapePipeline::get_Instance().destroy();
    TextPipeline::get_Instance().destroy();

    m_Window = nullptr;
}

void gear::Renderer::set_Camera(gear::Camera *camera)
{
    m_Camera = camera;
}

void gear::Renderer::set_V_Sync(bool v_sync)
{
    glfwSwapInterval(v_sync ? 1 : 0);
}

void gear::Renderer::start_New_Frame(void)
{
    glClearColor(m_Clear_Color[0], m_Clear_Color[1], m_Clear_Color[2], m_Clear_Color[3]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer.m_Framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gear::Renderer::show_Frame(void)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_Framebuffer.m_Texture);
    UpscalePipeline::get_Instance().bind();
    int window_Width, window_Height;
    glfwGetWindowSize(m_Window, &window_Width, &window_Height);
    glViewport(m_Top_Left[0], window_Height - m_Top_Left[1] - m_Bottom_Right[1], m_Bottom_Right[0] - m_Top_Left[0], m_Bottom_Right[1] - m_Top_Left[1]);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(m_Window);
}

void gear::Renderer::set_Viewport(Vector<int, 2> top_Left, Vector<int, 2> bottom_Right)
{
    m_Top_Left = top_Left;
    m_Bottom_Right = bottom_Right;
}

void gear::Renderer::set_Clear_Color(Vector<float, 4> color)
{
    m_Clear_Color = color;
}

gear::Vector<float, 4> gear::Renderer::get_Clear_Color(void)
{
    return m_Clear_Color;
}

void gear::Renderer::render_Scene(gear::Scene *scene)
{
    m_Framebuffer.bind();

    SpritePipeline::get_Instance().bind();
    glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
    glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);

    if (m_Camera != nullptr)
        glUniform2f(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Camera_Pos"), m_Camera->get_Position()[0], m_Camera->get_Position()[1]);
    else
        glUniform2f(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Camera_Pos"), 0, 0);

    SpritePipeline::get_Instance().render(scene);

    TextPipeline::get_Instance().bind();
    glUniform1i(glGetUniformLocation(TextPipeline::get_Instance().m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
    glUniform1i(glGetUniformLocation(TextPipeline::get_Instance().m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);

    if (m_Camera != nullptr)
        glUniform2f(glGetUniformLocation(TextPipeline::get_Instance().m_Shader, "u_Camera_Pos"), int(m_Camera->get_Position()[0] + 0.5), int(m_Camera->get_Position()[1] + 0.5));
    else
        glUniform2f(glGetUniformLocation(TextPipeline::get_Instance().m_Shader, "u_Camera_Pos"), 0, 0);

    TextPipeline::get_Instance().render(scene);

    continue_Animations(scene);
}

void gear::Renderer::render_UI(void)
{
    auto container = gear::UI::get_Container();

    m_Framebuffer.bind();

    SpritePipeline::get_Instance().bind();
    glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
    glUniform1i(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);
    
    glUniform2f(glGetUniformLocation(SpritePipeline::get_Instance().m_Shader, "u_Camera_Pos"), m_Framebuffer.m_Width / 2, m_Framebuffer.m_Height / 2);

    SpritePipeline::get_Instance().render_UI(container);

}

void gear::Renderer::render_Shape(const gear::Shape *shape, const Vector<float, 4> &color)
{
    if (!shape->is_Enabled())
        return;
    m_Framebuffer.bind();
    ShapePipeline::get_Instance().bind(shape->get_Type());
    glUniform1i(glGetUniformLocation(ShapePipeline::get_Instance().m_Shader, "u_Frame_Width"), m_Framebuffer.m_Width);
    glUniform1i(glGetUniformLocation(ShapePipeline::get_Instance().m_Shader, "u_Frame_Height"), m_Framebuffer.m_Height);

    if (m_Camera != nullptr)
        glUniform2f(glGetUniformLocation(ShapePipeline::get_Instance().m_Shader, "u_Camera_Pos"), int(m_Camera->get_Position()[0] + 0.5), int(m_Camera->get_Position()[1] + 0.5));
    else
        glUniform2f(glGetUniformLocation(ShapePipeline::get_Instance().m_Shader, "u_Camera_Pos"), 0, 0);
    ShapePipeline::get_Instance().render_Shape(shape, color);
}

static void dynamic_Physics_Collider_Render_Callback(gear::Entity parent, gear::DynamicPhysicsComponent &physics_Comp)
{
    for (auto &shape : physics_Comp.collider->get_Shapes())
        gear::Renderer::render_Shape(shape.absolute_Shape.get(), {0, 0.8, 1, 1});
}

static void static_Physics_Collider_Render_Callback(gear::Entity parent, gear::StaticPhysicsComponent &physics_Comp)
{
    for (auto &shape : physics_Comp.collider->get_Shapes())
        gear::Renderer::render_Shape(shape.absolute_Shape.get(), {0.7, 0, 1, 1});
}

static void hitbox_Render_Callback(gear::Entity parent, gear::HitboxComponent &hitbox_Comp)
{
    for (auto &hitbox : hitbox_Comp.hitboxes)
        for (auto &shape : hitbox->get_Shapes())
            gear::Renderer::render_Shape(shape.absolute_Shape.get(), {0.8, 0, 0, 1});
}

static void hurtbox_Render_Callback(gear::Entity parent, gear::HurtboxComponent &hurtbox_Comp)
{
    for (auto &hitbox : hurtbox_Comp.hurtboxes)
        for (auto &shape : hitbox->get_Shapes())
            gear::Renderer::render_Shape(shape.absolute_Shape.get(), {0, 0.8, 0, 1});
}

void gear::Renderer::render_All_Hitboxes(gear::Scene *scene)
{
    gear::Entity::for_Each(scene->get_ID(), dynamic_Physics_Collider_Render_Callback);
    gear::Entity::for_Each(scene->get_ID(), static_Physics_Collider_Render_Callback);
    gear::Entity::for_Each(scene->get_ID(), hitbox_Render_Callback);
    gear::Entity::for_Each(scene->get_ID(), hurtbox_Render_Callback);
}