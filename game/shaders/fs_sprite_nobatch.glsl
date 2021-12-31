#version 330 core

layout(location = 0) out vec4 out_Color;

uniform vec4 u_Palette[255];
uniform usampler2D u_Texture;

in vec2 tex_Position;

void main()
{
  uint index = texture(u_Texture, tex_Position).r;
  if(index == uint(0))
    discard;
  else
    out_Color = u_Palette[index];
}