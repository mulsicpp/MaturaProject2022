#version 430 core

layout(location = 0) out vec4 out_Color;

layout(binding = 0) uniform sampler2D u_Texture;
layout(binding = 1) uniform sampler1D u_Palette;

in vec2 tex_Position;

void main()
{
  int index = int(texture(u_Texture, tex_Position).r * 255.0f + 0.5);
  if(index == 0)
  {
    discard;
  }
  else
  //out_Color = vec4((index & 255u) / 255.0f, 0, 0, 1.0);
  out_Color = texture(u_Palette, (float(index) - 0.5f) / 255.0f);
  //out_Color = vec4(tex_Position.x, tex_Position.y, 0, 1);
}