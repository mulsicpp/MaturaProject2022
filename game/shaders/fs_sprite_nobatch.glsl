#version 330 core

layout(location = 0) out vec4 out_Color;

uniform vec4 u_Palette[255];
uniform sampler2D u_Texture;

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
  out_Color = u_Palette[index - 1];
  //out_Color = vec4(tex_Position.x, tex_Position.y, 0, 1);
}