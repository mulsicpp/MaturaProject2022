#pragma once

#include "converter.h"
#include <gear/data/Grid.h>

class AnimationConverter : public Converter
{
private:
    gear::Grid<uint32_t> m_Image;
    uint32_t *m_Palette = nullptr;
    uint32_t m_Palette_Size = 0;
    uint32_t m_Background = 0;

    bool m_Vertical = true;
    uint16_t m_Frame_Count = 0;
    float m_Frame_Rate = 0;

    uint8_t m_Type = 0;

protected:
    void command(int argc, char **argv);
    void convert(void);

public:
    AnimationConverter(FILE *file_In, gear::FileStream *file_Out);
};