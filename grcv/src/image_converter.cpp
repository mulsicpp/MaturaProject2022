#include "image_converter.h"
#include "ext/stb_image.h"

ImageConverter::ImageConverter(FILE *file_In, gear::FileStream *file_Out) : Converter(file_In, file_Out) {}

void ImageConverter::command(int argc, char **argv)
{
  if (argc > 0)
  {
    if (strcmp(argv[0], "img") == 0 && argc == 2)
    {
      int width, height, channels;
      uint8_t *data = stbi_load(argv[1], &width, &height, &channels, 4);
      m_Image = gear::Grid<uint32_t>(width, height, (uint32_t *)data);
      stbi_image_free(data);
      printf("img\n");
    }
    else if (strcmp(argv[0], "palette") == 0 && argc == 2)
    {
      int width, height, channels;
      m_Palette = (uint32_t*)stbi_load(argv[1], &width, &height, &channels, 4);
      m_Palette_Size = width * height;
      printf("palette\n");
    }
    else if (strcmp(argv[0], "background") == 0 && argc == 2)
    {
      printf("background begin\n");
      uint8_t *color_Bytes = (uint8_t *)&m_Background;
      for (int i = 0; i < 4; i++)
        color_Bytes[i] = 16 * (argv[1][2 * i] >= 'a' ? (argv[1][2 * i] - 'a' + 10) : (argv[1][2 * i] - '0')) + (argv[1][2 * i + 1] >= 'a' ? (argv[1][2 * i + 1] - 'a' + 10) : (argv[1][2 * i + 1] - '0'));
      printf("background\n");
    }
  }
}

void ImageConverter::convert(void)
{
  m_File_Out->put<uint16_t>(m_Image.width());
  m_File_Out->put<uint16_t>(m_Image.height());
  int size = m_Image.count();
  uint32_t *data = m_Image.data();
  for(int i = 0; i < size; i++)
  {
    if(data[i] == m_Background)
    {
      m_File_Out->put<uint8_t>(0);
    }
    int j;
    for(j = 0; j < m_Palette_Size; j++)
      if(data[i] == m_Palette[j])
      {
        m_File_Out->put<uint8_t>(j + 1);
        break;
      }
    if(j == m_Palette_Size)
      m_File_Out->put<uint8_t>(0);

  }
  stbi_image_free(m_Palette);
}