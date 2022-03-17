#pragma once

#include "converter.h"
#include <gear/data/Grid.h>

class PaletteConverter : public Converter
{
private:
    gear::Grid<uint32_t> m_Image;

protected:
    void command(int argc, char **argv);
    void convert(void);

public:
    PaletteConverter(FILE *file_In, gear::FileStream *file_Out);
};