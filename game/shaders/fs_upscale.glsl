#version 330 core

layout(location = 0) out vec4 out_Color;

uniform sampler2D u_Texture;

in vec2 tex_Position;

void main()
{
  out_Color = texture(u_Texture, tex_Position);
}