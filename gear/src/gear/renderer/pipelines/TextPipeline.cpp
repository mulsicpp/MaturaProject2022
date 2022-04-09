#include "TextPipeline.h"

#include <gear/scene/Entity.h>

#include "../shaders.h"

gear::TextPipeline gear::TextPipeline::instance;

gear::TextPipeline &gear::TextPipeline::get_Instance(void)
{
    return instance;
}

void gear::TextPipeline::init(void)
{
    unsigned int vertex_Shader = get_Shader(SHADER_TEXT_VS, GL_VERTEX_SHADER);
    unsigned int fragment_Shader = get_Shader(SHADER_TEXT_FS, GL_FRAGMENT_SHADER);

    create(vertex_Shader, fragment_Shader);
    bind();

    glUniform1i(glGetUniformLocation(m_Shader, "u_Texture"), 0);

    validate_Program(m_Shader);

    GEAR_DEBUG_LOG("opengl program: %i", m_Shader);

    glDeleteShader(vertex_Shader);
    glDeleteShader(fragment_Shader);

    GEAR_DEBUG_LOG("Vertex attribs");

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(2, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, parallax_Factor));
    glVertexAttribBinding(2, 0);
    glEnableVertexAttribArray(2);
    for (int i = 0; i < 4; i++)
    {
        glVertexAttribFormat(3 + i, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, colors) + 4 * i);
        glVertexAttribBinding(3 + i, 0);
        glEnableVertexAttribArray(3 + i);
    }

    glBindVertexBuffer(0, m_Vertexbuffer, 0, sizeof(Vertex));

    GEAR_DEBUG_LOG("Finished text pipeline");
}

void gear::TextPipeline::generate_Buffers(CachedText *data)
{
    data->char_Count = 1;
    Vertex vertices[4];

    auto char_Bounds = data->state.font->get_Char(*(data->state.text));
    auto colors = data->state.font->get_Colors();
    int width = data->state.font->get_Width(), height = data->state.font->get_Height();
    vertices[0] = Vertex{{0, 0, 0.5}, {char_Bounds->x_Start / float(width), char_Bounds->y_Start / float(height)}, 1};
    memcpy<uint8_t>((uint8_t *)vertices->colors, (uint8_t *)colors.data(), 16);

    vertices[1] = Vertex{{0, float(char_Bounds->y_End - char_Bounds->y_Start), 0.5}, {char_Bounds->x_Start / float(width), char_Bounds->y_End / float(height)}, 1};
    memcpy<uint8_t>((uint8_t *)vertices->colors, (uint8_t *)colors.data(), 16);

    vertices[2] = Vertex{{float(char_Bounds->x_End - char_Bounds->x_Start), float(char_Bounds->y_End - char_Bounds->y_Start), 0.5}, {char_Bounds->x_End / float(width), char_Bounds->y_End / float(height)}, 1};
    memcpy<uint8_t>((uint8_t *)vertices->colors, (uint8_t *)colors.data(), 16);

    vertices[3] = Vertex{{float(char_Bounds->x_End - char_Bounds->x_Start), 0, 0.5}, {char_Bounds->x_End / float(width), char_Bounds->y_Start / float(height)}, 1};
    memcpy<uint8_t>((uint8_t *)vertices->colors, (uint8_t *)colors.data(), 16);

    glBindBuffer(GL_ARRAY_BUFFER, data->vertex_Buffer_ID);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    unsigned int indices[6] = {0, 1, 2, 0, 2, 3};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->index_Buffer_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

void gear::TextPipeline::render_Text(Entity parent, TextComponent &text, TransformComponent &transform)
{

    if (instance.m_Cache[parent.get_Scene_ID()].find(parent.get_Entity_ID()) == instance.m_Cache[parent.get_Scene_ID()].end())
    {
        CachedText data;
        data.state = text;
        glGenBuffers(1, &data.vertex_Buffer_ID);
        glGenBuffers(1, &data.index_Buffer_ID);

        instance.generate_Buffers(&data);

        instance.m_Cache[parent.get_Scene_ID()].insert({parent.get_Entity_ID(), data});
    }
    else
    {
        TextComponent &text_Other = instance.m_Cache[parent.get_Scene_ID()][parent.get_Entity_ID()].state;
        if (text.font != text_Other.font ||
            text.height != text_Other.height ||
            text.offset != text_Other.offset ||
            text.raw_Text != text_Other.raw_Text ||
            text.width != text_Other.width ||
            strcmp(text.text, text_Other.text) != 0)
        {
            instance.generate_Buffers(&(instance.m_Cache[parent.get_Scene_ID()][parent.get_Entity_ID()]));
        }
    }

    CachedText &cached_Text = instance.m_Cache[parent.get_Scene_ID()][parent.get_Entity_ID()];
    Matrix<float, 3, 3> mat = transform.get_Matrix().cast_To<float, 3, 3>();

    glUniformMatrix3fv(glGetUniformLocation(instance.m_Shader, "u_Transform"), 9, GL_TRUE, (const float *)&mat);

    glBindBuffer(GL_ARRAY_BUFFER, cached_Text.vertex_Buffer_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cached_Text.index_Buffer_ID);

    glDrawElements(GL_TRIANGLES, 6 * cached_Text.char_Count, GL_UNSIGNED_INT, 0);
}

void gear::TextPipeline::render(gear::Scene *scene)
{
    Entity::for_Each(scene->get_ID(), render_Text);
}