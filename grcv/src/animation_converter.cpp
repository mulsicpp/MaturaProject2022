#include "animation_converter.h"
#include "ext/stb_image.h"

AnimationConverter::AnimationConverter(FILE *file_In, gear::FileStream *file_Out) : Converter(file_In, file_Out) {}

void AnimationConverter::command(int argc, char **argv)
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
        else if (strcmp(argv[0], "palette") == 0 && argc == 2)
        {
            int width, height, channels;
            m_Palette = (uint32_t *)stbi_load(argv[1], &width, &height, &channels, 4);
            m_Palette_Size = width * height;
        }
        else if (strcmp(argv[0], "background") == 0 && argc == 2)
        {
            uint8_t *color_Bytes = (uint8_t *)&m_Background;
            for (int i = 0; i < 4; i++)
                color_Bytes[i] = 16 * (argv[1][2 * i] >= 'a' ? (argv[1][2 * i] - 'a' + 10) : (argv[1][2 * i] - '0')) + (argv[1][2 * i + 1] >= 'a' ? (argv[1][2 * i + 1] - 'a' + 10) : (argv[1][2 * i + 1] - '0'));
        }
        else if (strcmp(argv[0], "type") == 0 && argc == 2)
        {
            if(strcmp(argv[1], "loop") == 0) {
                m_Type = 0;
            }
            else if(strcmp(argv[1], "forward") == 0) {
                m_Type = 1;
            }
            else if(strcmp(argv[1], "ping_pong") == 0) {
                m_Type = 2;
            }
        }
        else if (strcmp(argv[0], "frame_rate") == 0 && argc == 2)
        {
            m_Frame_Rate = atof(argv[1]);
        }
        else if (strcmp(argv[0], "frame_alignment") == 0 && argc == 2)
        {
            m_Vertical = argv[1][0] != 'h';
        }
        else if (strcmp(argv[0], "frame_count") == 0 && argc == 2)
        {
            m_Frame_Count = atoi(argv[1]);
        }
    }
}

void AnimationConverter::convert(void)
{
    int width, height;
    if (m_Vertical)
    {
        width = m_Image.width();
        height = m_Image.height() / m_Frame_Count;
    }
    else
    {
        width = m_Image.width() / m_Frame_Count;
        height = m_Image.height();
    }
    printf("%i %i %i %i\n", width, height, m_Frame_Count, m_Vertical);
    int size = m_Image.count();
    uint32_t *data = m_Image.data();
    uint8_t *compressed_Data = new uint8_t[size];
    for (int i = 0; i < size; i++)
    {
        if (data[i] == m_Background)
        {
            compressed_Data[i] = 0;
            continue;
        }
        int j;
        for (j = 0; j < m_Palette_Size; j++)
            if (data[i] == m_Palette[j])
            {
                compressed_Data[i] = j + 1;
                break;
            }
        if (j == m_Palette_Size)
            compressed_Data[i] = 0;
    }

    m_File_Out->put<uint16_t>(width);
    m_File_Out->put<uint16_t>(height);
    m_File_Out->put<uint16_t>(m_Frame_Count);
    m_File_Out->put<uint8_t>(m_Type);
    m_File_Out->put<float>(m_Frame_Rate);

    if (m_Vertical)
    {
        for (int i = 0; i < size; i++)
            m_File_Out->put<uint8_t>(compressed_Data[i]);
    }
    else
    {
        for (int i = 0; i < m_Frame_Count; i++)
            for (int j = 0; j < height; j++)
                for (int k = 0; k < width; k++)
                    m_File_Out->put<uint8_t>(compressed_Data[k + width * i + width * m_Frame_Count * j]);
    }

    delete[] compressed_Data;
    stbi_image_free(m_Palette);
}