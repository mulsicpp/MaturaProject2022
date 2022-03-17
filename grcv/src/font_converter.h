#pragma once

#include "converter.h"
#include <gear/data/Grid.h>

class FontConverter : public Converter
{
private:
    gear::Grid<uint32_t> m_Image;
    uint32_t m_Border_Color = 0;
    uint8_t m_Color_Count = 0;
    uint32_t *m_Colors = nullptr;
    uint8_t m_Flags = 0;
    uint16_t m_Row_Height = 0;
    std::vector<uint8_t> *m_Pixel_Rows = nullptr;
    int16_t m_Char_Gap = 0, m_Line_Gap = 0;

    uint8_t m_Char_Count = 0;
    char m_Characters[256];

protected:
    void command(int argc, char **argv);
    void convert(void);

public:
    FontConverter(FILE *file_In, gear::FileStream *file_Out);
};