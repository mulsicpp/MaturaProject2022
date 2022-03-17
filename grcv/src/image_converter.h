#pragma once

#include "converter.h"
#include <gear/data/Grid.h>

class ImageConverter : public Converter
{
private:
    gear::Grid<uint32_t> m_Image;
    uint32_t *m_Palette = nullptr;
    uint32_t m_Palette_Size = 0;
    uint32_t m_Background = 0;

protected:
    void command(int argc, char **argv);
    void convert(void);

public:
    ImageConverter(FILE *file_In, gear::FileStream *file_Out);
};