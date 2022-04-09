#include "ShapePipeline.h"
#include "../shaders.h"
#include <gear/math/constants.h>

typedef void (gear::Shape::*shapeMember)(void);

gear::ShapePipeline gear::ShapePipeline::m_Instance;

gear::ShapePipeline &gear::ShapePipeline::get_Instance(void)
{
    return m_Instance;
}

void gear::ShapePipeline::init(void)
{
    unsigned int vertex_Shader = get_Shader(SHADER_SHAPE_VS, GL_VERTEX_SHADER);
    unsigned int fragment_Shader = get_Shader(SHADER_SHAPE_FS, GL_FRAGMENT_SHADER);

    m_Shader = link_Program(vertex_Shader, fragment_Shader);
    glCreateVertexArrays(1, &m_Vertexarray);

    validate_Program(m_Shader);
    RenderPipeline::bind();
    GEAR_DEBUG_LOG("opengl program: %i", m_Shader);

    glDeleteShader(vertex_Shader);
    glDeleteShader(fragment_Shader);

    glCreateBuffers(1, &m_Rect_Data);
    glBindBuffer(GL_ARRAY_BUFFER, m_Rect_Data);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glCreateBuffers(1, &m_Circle_Data);
    glBindBuffer(GL_ARRAY_BUFFER, m_Circle_Data);
    glBufferData(GL_ARRAY_BUFFER, 512 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glCreateBuffers(1, &m_Point_Data);
    glBindBuffer(GL_ARRAY_BUFFER, m_Point_Data);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribFormat(0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);
}

void gear::ShapePipeline::destroy(void)
{
    glDeleteBuffers(1, &m_Rect_Data);
    glDeleteBuffers(1, &m_Circle_Data);
    glDeleteBuffers(1, &m_Point_Data);

    glDeleteVertexArrays(1, &m_Vertexarray);
    glDeleteProgram(m_Shader);
}

void gear::ShapePipeline::bind(gear::ShapeType type)
{
    RenderPipeline::bind();
    switch (type)
    {
    case ShapeType::RECT:
        glBindBuffer(GL_ARRAY_BUFFER, m_Rect_Data);
        glBindVertexBuffer(0, m_Rect_Data, 0, sizeof(float) * 2);
        break;
    case ShapeType::CIRCLE:
        glBindBuffer(GL_ARRAY_BUFFER, m_Circle_Data);
        glBindVertexBuffer(0, m_Circle_Data, 0, sizeof(float) * 2);
        break;
    case ShapeType::POINT:
        glBindBuffer(GL_ARRAY_BUFFER, m_Point_Data);
        glBindVertexBuffer(0, m_Point_Data, 0, sizeof(float) * 2);
        break;
    }
}

void gear::ShapePipeline::render_Shape(const gear::Shape *shape, const gear::Vector<float, 4> &color)
{
    glUniform4f(glGetUniformLocation(m_Instance.m_Shader, "u_Color"), color[0], color[1], color[2], color[3]);
    switch (shape->get_Type())
    {
    case ShapeType::RECT:
        render_Rect((Rect *)shape);
        break;
    case ShapeType::CIRCLE:
        render_Circle((Circle *)shape);
        break;
    case ShapeType::POINT:
        render_Point((Point *)shape);
        break;
    }
}

void gear::ShapePipeline::render_Rect(const Rect *rect)
{
    float points[8] = {
        rect->top_Left[0], rect->top_Left[1],
        rect->bottom_Right[0], rect->top_Left[1],
        rect->bottom_Right[0], rect->bottom_Right[1],
        rect->top_Left[0], rect->bottom_Right[1]};

    glBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(float), points);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
}

void gear::ShapePipeline::render_Circle(const Circle *circle)
{
    float points[512];
    for (int i = 0; i < 256; i++)
    {
        points[2 * i] = sin(i * gear::PI * 2 / 256) * circle->radius + circle->position[0];
        points[2 * i + 1] = cos(i * gear::PI * 2 / 256) * circle->radius + circle->position[1];
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, 512 * sizeof(float), points);
    glDrawArrays(GL_LINE_LOOP, 0, 256);
}

void gear::ShapePipeline::render_Point(const Point *point)
{
    float points[2] = {
        point->position[0], point->position[1]};

    glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(float), points);
    glDrawArrays(GL_POINTS, 0, 1);
}