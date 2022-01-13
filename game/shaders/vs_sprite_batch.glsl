#version 430 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_Tex_Position;
layout(location = 2) flat in int in_Index;

uniform int u_Frame_Width;
uniform int u_Frame_Height;

out vec2 tex_Position;
flat out index;

void main()
{
  gl_Position = vec4((in_Position.x * 2.0f / float(u_Frame_Width)) - 1.0f, 1.0f - (in_Position.y * 2.0f / float(u_Frame_Height)), 0.0f, 1.0f);
  //gl_Position = vec4(in_Position.x / 1000.0f * u_Frame_Height, in_Position.y / 1000.0f * u_Frame_Width, 0.0f, 1.0f);
  tex_Position = in_Tex_Position;
  index = in_Index;
}