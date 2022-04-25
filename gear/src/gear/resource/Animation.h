#pragma once

#include <gear/core/core.h>
#include "Resource.h"

_GEAR_START

class Animation : public Resource
{
public:
    enum class Type
    {
        LOOP = 0,
        FORWARD = 1,
        PING_PONG = 2
    };

private:
    uint16_t m_Width, m_Height, m_Frame_Count;
    Type m_Type;
    float m_Frame_Rate;
    unsigned int m_TextureID;

protected:
    int load(FileStream *file_Stream) override;

public:
    Animation(void);
    ~Animation();

    Animation(const Animation &) = delete;
    Animation &operator=(const Animation &) = delete;

    uint16_t get_Width(void) const;
    uint16_t get_Height(void) const;
    uint16_t get_Frame_Count(void) const;

    Type get_Type(void) const;

    float get_Frame_Rate(void) const;

    unsigned int get_TextureID(void) const;
};

_GEAR_END