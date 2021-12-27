#include "palette_converter.h"
#include "ext/stb_image.h"

PaletteConverter::PaletteConverter(FILE *file_In, gear::FileStream *file_Out) : Converter(file_In, file_Out) {}

void PaletteConverter::command(int argc, char **argv)
{
  if (argc > 0)
  {
    if (strcmp(argv[0], "img") == 0 && argc == 2)
    {
      int width, height, channels;
      uint8_t *data = stbi_load(argv[1], &width, &height, &channels, 4);
      m_Image = gear::Grid<uint32_t>(width, height, (uint32_t *)data);
      stbi_image_free(data);
    }
  }
}

void PaletteConverter::convert(void)
{
  m_File_Out->put<uint8_t>(m_Image.count());
  uint32_t *data = m_Image.data();
  for(int i = 0; i < m_Image.count(); i++)
    m_File_Out->put<uint32_t>(data[i]);
}