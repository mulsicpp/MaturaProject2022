#version 430 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_Tex_Position;

out vec2 tex_Position;

void main()
{
  gl_Position = vec4(in_Position.x, in_Position.y, 0.0f, 1.0f);
  tex_Position = in_Tex_Position;
}