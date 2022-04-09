#include "Font.h"

#include <glad/glad.h>
#include <string.h>

gear::Font::Font(void) : m_TextureID(0), m_Flags(0), m_Char_Gap(0), m_Line_Gap(0), m_Width(0), m_Height(0) {}
gear::Font::~Font()
{
    GEAR_DEBUG_LOG("delete font");
    if (m_TextureID != 0)
        glDeleteTextures(1, &m_TextureID);
}

int gear::Font::load(FileStream *file_Stream)
{
    GEAR_DEBUG_LOG("loading font ...");
    if (file_Stream == nullptr)
        return -1;

    file_Stream->get<uint8_t>(&m_Flags);
    uint8_t color_Count = 0;
    file_Stream->get<uint8_t>(&color_Count);
    if (color_Count == 0 || color_Count > 255)
        return -2;

    uint8_t bits_Per_Pixel = 0;
    file_Stream->get<uint8_t>(&bits_Per_Pixel);

    file_Stream->get<int16_t>(&m_Char_Gap);
    file_Stream->get<int16_t>(&m_Line_Gap);

    file_Stream->get<uint16_t>(&m_Width);
    file_Stream->get<uint16_t>(&m_Height);

    uint32_t data_Size;
    file_Stream->get<uint32_t>(&data_Size);

    uint8_t char_Count = 0;
    file_Stream->get<uint8_t>(&char_Count);

    uint8_t color_Color = 0;
    file_Stream->get<uint8_t>(&color_Color);

    uint16_t offset = 0;
    char character;
    uint16_t char_Width;
    for (int i = 0; i < char_Count; i++)
    {
        file_Stream->get<char>(&character);
        file_Stream->get<uint16_t>(&char_Width);
        m_Characters[character] = {offset, (uint16_t)(offset + char_Width), 0, m_Height};
        offset += char_Width;
    }

    Vector<uint8_t, 4> color;
    for (int i = 0; i < color_Count; i++)
    {
        file_Stream->get<Vector<uint8_t, 4>>(&color);
        m_Colors.push_back(color);
        GEAR_DEBUG_LOG("[%i, %i, %i, %i]", color[0], color[1], color[2], color[3]);
    }



    uint8_t bit_Mask = 0;
    for (int i = 0; i < bits_Per_Pixel; i++)
        bit_Mask |= (1 << i);
    uint8_t pixels_Per_Byte = 8 / bits_Per_Pixel;

    uint32_t atlas_Count = m_Width * m_Height;
    uint8_t *atlas_Data = new uint8_t[atlas_Count];
    uint8_t current_Byte;
    for (int i = 0; i < atlas_Count; i++)
    {
        if (i % pixels_Per_Byte == 0)
            file_Stream->get<uint8_t>(&current_Byte);
        atlas_Data[i] = (current_Byte >> ((pixels_Per_Byte - 1 - (i % pixels_Per_Byte)) * bits_Per_Pixel)) & bit_Mask;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, atlas_Data);

    GEAR_DEBUG_LOG("Font atlas id: %i", m_TextureID);
}

uint8_t gear::Font::get_Flags(void) const { return m_Flags; }
uint16_t gear::Font::get_Char_Gap(void) const { return m_Char_Gap; }
uint16_t gear::Font::get_Line_Gap(void) const { return m_Line_Gap; }
uint16_t gear::Font::get_Width(void) const { return m_Width; }
uint16_t gear::Font::get_Height(void) const { return m_Height; }
unsigned int gear::Font::get_TextureID(void) const { return m_TextureID; }

const gear::Font::CharacterLocation *gear::Font::get_Char(unsigned int character) const
{
    if (character > 255 || m_Characters.find(character) == m_Characters.end())
        return &m_Characters.at(0);
    return &m_Characters.at(character);
}

const std::vector<gear::Vector<uint8_t, 4>> &gear::Font::get_Colors(void) const {
    return m_Colors;
}