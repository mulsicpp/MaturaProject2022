#pragma once

#include "converter.h"
#include <gear/data/Grid.h>

class ImageConverter : public Converter {
private:
  gear::Grid<uint32_t> m_Image;
  bool m_Auto_Gen_Palette = false;

protected:
  void command(int argc, char **argv);
  void convert(void);

public:
  ImageConverter(FILE *file_In, gear::FileStream *file_Out);
};