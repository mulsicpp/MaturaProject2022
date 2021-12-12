#include "font_converter.h"
#include "ext/stb_image.h"

FontConverter::FontConverter(FILE *file_In, gear::FileStream *file_Out) : Converter(file_In, file_Out) {}

void FontConverter::command(int argc, char **argv)
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
    else if (strcmp(argv[0], "border_color") == 0 && argc == 2)
    {
      uint8_t *border_Color_Bytes = (uint8_t *)&m_Border_Color;
      for (int i = 0; i < 4; i++)
        border_Color_Bytes[i] = 16 * (argv[1][2 * i] >= 'a' ? (argv[1][2 * i] - 'a' + 10) : (argv[1][2 * i] - '0')) + (argv[1][2 * i + 1] >= 'a' ? (argv[1][2 * i + 1] - 'a' + 10) : (argv[1][2 * i + 1] - '0'));
    }
    else if (strcmp(argv[0], "color_count") == 0 && argc == 2)
    {
      m_Color_Count = atoi(argv[1]);
      m_Colors = new uint32_t[m_Color_Count + 1];
    }
    else if (strcmp(argv[0], "color") == 0 && argc == 3)
    {
      uint8_t *color_Bytes = (uint8_t *)(m_Colors + atoi(argv[1]));
      for (int i = 0; i < 4; i++)
        color_Bytes[i] = 16 * (argv[2][2 * i] >= 'a' ? (argv[2][2 * i] - 'a' + 10) : (argv[2][2 * i] - '0')) + (argv[2][2 * i + 1] >= 'a' ? (argv[2][2 * i + 1] - 'a' + 10) : (argv[2][2 * i + 1] - '0'));
    }
    else if (strcmp(argv[0], "row_height") == 0 && argc == 2)
    {
      m_Row_Height = atoi(argv[1]);
      m_Pixel_Rows = new std::vector<uint8_t>[m_Row_Height];
    }
    else if (strcmp(argv[0], "char_gap") == 0 && argc == 2)
    {
      m_Char_Gap = atoi(argv[1]);
    }
    else if (strcmp(argv[0], "line_gap") == 0 && argc == 2)
    {
      m_Line_Gap = atoi(argv[1]);
    }
    else if (strcmp(argv[0], "char") == 0 && argc == 2)
    {
      m_Characters[m_Char_Count++] = argv[1][0];
    }
    else if (strcmp(argv[0], "char_array") == 0 && argc == 2)
    {
      gear::memcpy<char>(m_Characters + m_Char_Count, argv[1], strlen(argv[1]));
      m_Char_Count += strlen(argv[1]);
    }
    else if (strcmp(argv[0], "char_range") == 0 && argc == 3)
    {
      uint8_t char_Count = argv[2][0] - argv[1][0] + 1;
      for (int i = 0; i < char_Count; i++)
        m_Characters[m_Char_Count + i] = argv[1][0] + i;
      m_Char_Count += char_Count;
    }
    else if (strcmp(argv[0], "char_code") == 0 && argc == 2)
    {
      m_Characters[m_Char_Count++] = atoi(argv[1]);
    }
    else if (strcmp(argv[0], "char_code_array") == 0)
    {
      for (int i = 1; i < argc; i++)
        m_Characters[m_Char_Count++] = atoi(argv[i]);
    }
    else if (strcmp(argv[0], "char_code_range") == 0 && argc == 3)
    {
      uint8_t start = atoi(argv[1]), end = atoi(argv[2]);
      uint8_t char_Count = end - start + 1;
      for (int i = 0; i < char_Count; i++)
        m_Characters[m_Char_Count + i] = start + i;
      m_Char_Count += char_Count;
    }
  }
}

void FontConverter::convert(void)
{
  //printf("started conversion\n");
  uint32_t *img_Data = m_Image.data();
  int img_Width = m_Image.width(), img_Height = m_Image.height();

  int i = 0;
  int y = 0;

  uint16_t font_Width = 0;

  //printf("char count %i", m_Char_Count);

  uint16_t *char_Width = new uint16_t[m_Char_Count]{0};

  /*gear::FileStream *stream = gear::FileStream::open("test.txt", "w");

  for (int i = 0; i < img_Height; i++)
  {
    for (int j = 0; j < img_Width; j++)
    {
      uint32_t pixel = img_Data[j + i * img_Width];
      stream->printf("%s", pixel == 0xffffffff ? "  " : (pixel == 0xff000000 ? "MM" : (pixel == 0xff00ffff ? "--" : "==")));
    }
    stream->printf("\n");
  }

  gear::FileStream::close(stream);*/

  //printf("alogrithm started\n");

  while (y < img_Height && i < m_Char_Count)
  {
    if (img_Data[y * img_Width] == m_Border_Color)
    {
      //printf("skipped line\n");
      y++;
      continue;
    }
    for (int x = 0; x < img_Width; x++)
    {
      if (img_Data[x + y * img_Width] == m_Border_Color)
      {
        if (x != 0 && img_Data[x - 1 + y * img_Width] != m_Border_Color)
        {
          //printf("char range %i %i\n", i, char_Width[i]);
          i++;
        }
      }
      else
      {
        for (int dy = 0; dy < m_Row_Height; dy++)
          for (int j = 0; j < m_Color_Count + 1; j++)
            if (m_Colors[j] == img_Data[x + ((y + dy) * img_Width)])
            {
              //printf("%i", j);
              m_Pixel_Rows[dy].push_back(j);
              break;
            }

        //printf("\n");
        char_Width[i]++;
        if (x == img_Width - 1)
        {
          //printf("next char %i\n", char_Width[i]);
          i++;
        }
      }
    }
    //printf("next line\n");
    y += m_Row_Height;
  }

  //printf("algorithm finished\n");

  m_File_Out->put<uint8_t>(m_Flags);
  m_File_Out->put<uint8_t>(m_Color_Count);

  uint8_t bits_Per_Pixel = 0;

  if (m_Color_Count == 1)
    bits_Per_Pixel = 1;
  else if (m_Color_Count < 4)
    bits_Per_Pixel = 2;
  else if (m_Color_Count < 16)
    bits_Per_Pixel = 4;
  else
    bits_Per_Pixel = 8;

  uint8_t pixels_Per_Byte = 8 / bits_Per_Pixel;
  uint8_t pixel_Mask = 0;
  for (int i = 0; i < bits_Per_Pixel; i++)
    pixel_Mask |= (1 << i);

  m_File_Out->put<uint8_t>(bits_Per_Pixel);

  m_File_Out->put<int16_t>(m_Char_Gap);
  m_File_Out->put<int16_t>(m_Line_Gap);

  m_File_Out->put<uint16_t>(m_Pixel_Rows[0].size());
  m_File_Out->put<uint16_t>(m_Row_Height);

  uint32_t data_Size = (m_Pixel_Rows[0].size() * m_Row_Height + pixels_Per_Byte - 1) / pixels_Per_Byte;
  m_File_Out->put<uint32_t>(data_Size);

  m_File_Out->put<uint8_t>(m_Char_Count);
  for (int i = 0; i < m_Char_Count; i++)
  {
    m_File_Out->put<char>(m_Characters[i]);
    m_File_Out->put<uint16_t>(char_Width[i]);
  }

  uint32_t byte_Data_Size = m_Pixel_Rows[0].size() * m_Row_Height;
  uint8_t *data = new uint8_t[byte_Data_Size];
  for (int i = 0; i < m_Row_Height; i++)
    gear::memcpy(data + (m_Pixel_Rows[0].size() * i), m_Pixel_Rows[i].data(), m_Pixel_Rows[i].size());

  //printf("data compression\n");
  i = 0;
  uint8_t byte = 0;
  while (i < byte_Data_Size)
  {
    byte |= (data[i] << ((pixels_Per_Byte - 1 - (i % pixels_Per_Byte)) * bits_Per_Pixel));
    i++;
    if (i % pixels_Per_Byte == 0)
    {
      m_File_Out->put<uint8_t>(byte);
      byte = 0;
    }
  }
  if (i % pixels_Per_Byte != 0)
  {
    m_File_Out->put<uint8_t>(byte);
  }

  //printf("finished conversion\n");

  delete[] data;
  //printf("deleted data\n");
  delete[] char_Width;
  //printf("deleted char width\n");
  delete[] m_Colors;
  //printf("deleted colors\n");
  delete[] m_Pixel_Rows;
  //printf("deleted pixel rows\n");
}