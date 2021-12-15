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
    }
  }
}

void ImageConverter::convert(void)
{

}