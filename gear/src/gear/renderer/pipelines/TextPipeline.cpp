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
}

static int attempt_New_Line(gear::Vector<int, 2> *cursor, gear::TextComponent &text_Comp)
{
    if ((*cursor)[1] + 2 * text_Comp.font->get_Height() + text_Comp.font->get_Line_Gap() > text_Comp.height)
        return 1;
    else
    {
        (*cursor)[0] = 0;
        (*cursor)[1] += text_Comp.font->get_Height() + text_Comp.font->get_Line_Gap();
        return 0;
    }
}

int gear::TextPipeline::append_Char(CachedText *data, std::vector<Vertex> *vertices, Vector<int, 2> *cursor, char character)
{
    if (character == ' ')
    {
        (*cursor)[0] += data->state.font->get_Space_Gap();
        return 0;
    }
    else if (character == '\t')
    {
        (*cursor)[0] += data->state.font->get_Space_Gap() * 2;
        return 0;
    }
    else if (character == '\n')
    {
        return attempt_New_Line(cursor, data->state);
    }

    auto char_Bounds = data->state.font->get_Char(character);

    if ((*cursor)[0] + char_Bounds->x_End - char_Bounds->x_Start + data->state.font->get_Char_Gap() > data->state.width)
        if (attempt_New_Line(cursor, data->state))
            return 1;

    auto &colors = data->state.colors;

    int copy_Count = colors.size() < 4 ? colors.size() : 4;

    int width = data->state.font->get_Width(), height = data->state.font->get_Height();
    vertices->push_back(Vertex{{float(data->state.offset[0] + (*cursor)[0]), float(data->state.offset[1] + (*cursor)[1]), data->state.offset[2]}, {char_Bounds->x_Start / float(width), char_Bounds->y_Start / float(height)}, 1});
    memcpy<uint8_t>((uint8_t *)vertices->back().colors, (uint8_t *)colors.data(), 4 * copy_Count);

    vertices->push_back(Vertex{{float(data->state.offset[0] + (*cursor)[0]), float(data->state.offset[1] + (*cursor)[1] + char_Bounds->y_End - char_Bounds->y_Start), data->state.offset[2]}, {char_Bounds->x_Start / float(width), char_Bounds->y_End / float(height)}, 1});
    memcpy<uint8_t>((uint8_t *)vertices->back().colors, (uint8_t *)colors.data(), 4 * copy_Count);

    vertices->push_back(Vertex{{float(data->state.offset[0] + (*cursor)[0] + char_Bounds->x_End - char_Bounds->x_Start), float(data->state.offset[1] + (*cursor)[1] + char_Bounds->y_End - char_Bounds->y_Start), data->state.offset[2]}, {char_Bounds->x_End / float(width), char_Bounds->y_End / float(height)}, 1});
    memcpy<uint8_t>((uint8_t *)vertices->back().colors, (uint8_t *)colors.data(), 4 * copy_Count);

    vertices->push_back(Vertex{{float(data->state.offset[0] + (*cursor)[0] + char_Bounds->x_End - char_Bounds->x_Start), float(data->state.offset[1] + (*cursor)[1]), data->state.offset[2]}, {char_Bounds->x_End / float(width), char_Bounds->y_Start / float(height)}, 1});
    memcpy<uint8_t>((uint8_t *)vertices->back().colors, (uint8_t *)colors.data(), 4 * copy_Count);

    data->char_Count++;
    (*cursor)[0] += char_Bounds->x_End - char_Bounds->x_Start + data->state.font->get_Char_Gap();
    return 0;
}

int gear::TextPipeline::append_Char_Virtual(CachedText *data, Vector<int, 2> *cursor, char character)
{
    auto char_Bounds = data->state.font->get_Char(character);

    if ((*cursor)[0] + char_Bounds->x_End - char_Bounds->x_Start + data->state.font->get_Char_Gap() > data->state.width)
        return 1;
    
    (*cursor)[0] += char_Bounds->x_End - char_Bounds->x_Start + data->state.font->get_Char_Gap();
    return 0;
}

void gear::TextPipeline::generate_Buffers(CachedText *data)
{
    GEAR_DEBUG_LOG("generating vertex data");
    data->char_Count = 0;
    std::vector<Vertex> vertices;

    Vector<int, 2> cursor;
    const char *text_Data = data->state.text.data();

    if (data->state.break_Word)
    {
        for (int i = 0; text_Data[i] != 0; i++)
            if (append_Char(data, &vertices, &cursor, text_Data[i]))
                break;
    }
    else
    {
        Vector<int, 2> virtual_Cursor;
        bool start = true;
        for (int i = 0; text_Data[i] != 0; i++)
        {
            if (text_Data[i] != ' ' && text_Data[i] != '\n' && text_Data[i] != '\t')
            {
                if(start) {
                    virtual_Cursor = cursor;
                    bool nl_Required = false;
                    for(int j = 0; text_Data[i + j] != ' ' && text_Data[i + j] != '\n' && text_Data[i + j] != '\t' && text_Data[i + j] != 0; j++) {
                        if(append_Char_Virtual(data, &virtual_Cursor, text_Data[i + j])) {
                            nl_Required = true;
                            break;
                        }
                    }
                    if(nl_Required)
                        attempt_New_Line(&cursor, data->state);
                }
                start = false;
                if (append_Char(data, &vertices, &cursor, text_Data[i]))
                    break;
            }
            else
            {
                if (append_Char(data, &vertices, &cursor, text_Data[i]))
                    break;
                start = true;
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, data->vertex_Buffer_ID);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex) * data->char_Count, vertices.data(), GL_STATIC_DRAW);

    glBindVertexBuffer(0, data->vertex_Buffer_ID, 0, sizeof(Vertex));

    unsigned int *indices = new unsigned int[6 * data->char_Count];
    for (int i = 0; i < data->char_Count; i++)
    {
        indices[i * 6] = i * 4;
        indices[i * 6 + 1] = i * 4 + 1;
        indices[i * 6 + 2] = i * 4 + 2;
        indices[i * 6 + 3] = i * 4;
        indices[i * 6 + 4] = i * 4 + 2;
        indices[i * 6 + 5] = i * 4 + 3;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->index_Buffer_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int) * data->char_Count, indices, GL_STATIC_DRAW);
}

void gear::TextPipeline::render_Text(Entity parent, TextComponent &text, TransformComponent &transform)
{
    if (instance.m_Cache[parent.get_Scene_ID()].find(parent.get_Entity_ID()) == instance.m_Cache[parent.get_Scene_ID()].end())
    {
        CachedText data;
        data.state = text;
        glGenBuffers(1, &data.vertex_Buffer_ID);
        glGenBuffers(1, &data.index_Buffer_ID);

        GEAR_DEBUG_LOG("colors[0]: {%i, %i, %i, %i}", data.state.colors[0][0], data.state.colors[0][1], data.state.colors[0][2], data.state.colors[0][3]);
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
            text.text != text_Other.text ||
            text.colors != text_Other.colors ||
            text.break_Word != text_Other.break_Word)
        {
            instance.m_Cache[parent.get_Scene_ID()][parent.get_Entity_ID()].state = text;
            auto data = instance.m_Cache[parent.get_Scene_ID()][parent.get_Entity_ID()];
            GEAR_DEBUG_LOG("colors[0]: {%i, %i, %i, %i}", data.state.colors[0][0], data.state.colors[0][1], data.state.colors[0][2], data.state.colors[0][3]);
            instance.generate_Buffers(&(instance.m_Cache[parent.get_Scene_ID()][parent.get_Entity_ID()]));
        }
    }

    CachedText &cached_Text = instance.m_Cache[parent.get_Scene_ID()][parent.get_Entity_ID()];
    Matrix<float, 3, 3> mat = transform.get_Matrix().cast_To<float, 3, 3>();

    glUniformMatrix3fv(glGetUniformLocation(instance.m_Shader, "u_Transform"), 1, GL_TRUE, (const float *)&mat);

    auto &colors = cached_Text.state.font->get_Colors();

    float *float_Data = new float[colors.size() * 4];

    for (int i = 0; i < colors.size(); i++)
    {
        float_Data[i * 4] = colors[i][0] / 255.0f;
        float_Data[i * 4 + 1] = colors[i][1] / 255.0f;
        float_Data[i * 4 + 2] = colors[i][2] / 255.0f;
        float_Data[i * 4 + 3] = colors[i][3] / 255.0f;
    }

    glUniform4fv(glGetUniformLocation(instance.m_Shader, "u_Palette"), colors.size(), float_Data);

    delete[] float_Data;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cached_Text.state.font->get_TextureID());

    glBindBuffer(GL_ARRAY_BUFFER, cached_Text.vertex_Buffer_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cached_Text.index_Buffer_ID);

    int vertex_Size, index_Size;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vertex_Size);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &index_Size);

    glDrawElements(GL_TRIANGLES, 6 * cached_Text.char_Count, GL_UNSIGNED_INT, 0);
}

void gear::TextPipeline::render(gear::Scene *scene)
{
    bind();
    Entity::for_Each(scene->get_ID(), render_Text);
}

void gear::TextPipeline::clear_Cache(unsigned int entityID)
{
    m_Cache->erase(entityID);
}

void gear::_clear_Text_Cache(unsigned int entityID)
{
    TextPipeline::get_Instance().clear_Cache(entityID);
}