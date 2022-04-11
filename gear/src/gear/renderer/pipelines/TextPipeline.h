#pragma once

#include <gear/core/core.h>
#include "../RenderPipeline.h"
#include <gear/math/Vector.h>

#include <gear/scene/Scene.h>

#include <gear/scene/TransformComponent.h>
#include "../TextComponent.h"

#include <unordered_map>

#include <gear/core/debug/log.h>

_GEAR_START

class TextPipeline : public RenderPipeline
{
private:
    struct Vertex
    {
        Vector<float, 3> pos;
        Vector<float, 2> tex;
        float parallax_Factor;
        Vector<uint8_t, 4> colors[4];
    };

    struct CachedText {
        TextComponent state;
        unsigned int vertex_Buffer_ID;
        unsigned int index_Buffer_ID;
        unsigned int char_Count;
    };

    std::unordered_map<uint32_t, CachedText> m_Cache[GEAR_MAX_SCENES];

    static TextPipeline instance;

    static void generate_Buffers(CachedText *data);
    static void append_Char(CachedText *data, std::vector<Vertex> *vertices, Vector<int, 2> *cursor, char character);
    static void render_Text(Entity parent, TextComponent &text, TransformComponent &transform);

public:
    static TextPipeline &get_Instance(void);

    void init(void) override;

    void render(Scene *scene);

    void clear_Cache(unsigned int entityID);
};

_GEAR_END