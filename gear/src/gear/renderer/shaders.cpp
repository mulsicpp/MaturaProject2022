#include "shaders.h"

_GEAR_START

char shader_Buffer[GEAR_MAX_SHADER_CODE_LENGTH];

const char *const SHADER_UPSCALE_VS = R"(
#version 330 core

layout(location = 0) in vec2 in_Position;
layout(location = 1) in vec2 in_Tex_Position;

out vec2 tex_Position;

void main() {
  gl_Position = vec4(in_Position.x, in_Position.y, 0.0f, 1.0f);
  tex_Position = in_Tex_Position;
}
)";

const char *const SHADER_UPSCALE_FS = R"(
#version 330 core

layout(location = 0) out vec4 out_Color;

uniform sampler2D u_Texture;

in vec2 tex_Position;

void main() {
  out_Color = texture(u_Texture, tex_Position);
}
)";



const char *const SHADER_SPRITE_VS = R"(
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Tex_Position;
layout(location = 2) in float in_Index;
layout(location = 3) in float in_Parallax;

uniform int u_Frame_Width;
uniform int u_Frame_Height;

uniform vec2 u_Camera_Pos;

out vec2 tex_Position;
out float tex_Index;

void main() {
  gl_Position = vec4((in_Position.x - u_Camera_Pos.x) * in_Parallax * 2.0f / float(u_Frame_Width), -(in_Position.y - u_Camera_Pos.y) * in_Parallax * 2.0f / float(u_Frame_Height), in_Position.z, 1.0f);
  tex_Position = in_Tex_Position;
  tex_Index = in_Index;
}
)";

const char *const SHADER_SPRITE_FS = R"(
#version 330 core

layout(location = 0) out vec4 out_Color;

uniform sampler2D u_Texture[%i];
uniform sampler1D u_Palette[%i];

in vec2 tex_Position;
in float tex_Index;

void main() {
  int index = int(texture(u_Texture[int(tex_Index)], tex_Position).r * 255.0f + 0.5);
  if(index == 0)
  {
    discard;
  }
  else
  out_Color = texture(u_Palette[int(tex_Index)], (float(index) - 0.5f) / 255.0f);
  //out_Color = vec4(vec3(gl_FragCoord.z), 1.0);
}
)";



const char *const SHADER_SHAPE_VS = R"(
#version 330 core

layout(location = 0) in vec2 in_Position;

uniform int u_Frame_Width;
uniform int u_Frame_Height;

uniform vec2 u_Camera_Pos;

void main() {
  gl_Position = vec4((in_Position.x - u_Camera_Pos.x) * 2.0f / float(u_Frame_Width), -(in_Position.y - u_Camera_Pos.y) * 2.0f / float(u_Frame_Height), 1.0, 1.0);
}
)";

const char *const SHADER_SHAPE_FS = R"(
#version 330 core

layout(location = 0) out vec4 out_Color;

uniform vec4 u_Color;

void main() {
  out_Color = u_Color;
}
)";



const char *const SHADER_TEXT_VS = R"(
#version 330 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 in_Tex_Position;
layout(location = 2) in float in_Parallax;
layout(location = 3) in vec4 in_Color_0;
layout(location = 4) in vec4 in_Color_1;
layout(location = 5) in vec4 in_Color_2;
layout(location = 6) in vec4 in_Color_3;

uniform int u_Frame_Width;
uniform int u_Frame_Height;

uniform vec2 u_Camera_Pos;

uniform mat3 u_Transform;

out vec2 tex_Position;
out vec4 colors[4];

void main() {
  vec3 temp = u_Transform * vec3(in_Position.x, in_Position.y, 1);
  gl_Position = vec4((temp.x - u_Camera_Pos.x) * in_Parallax * 2.0f / float(u_Frame_Width), -(temp.y - u_Camera_Pos.y) * in_Parallax * 2.0f / float(u_Frame_Height), in_Position.z, 1.0f);
  tex_Position = in_Tex_Position;
  colors[0] = in_Color_0;
  colors[1] = in_Color_1;
  colors[2] = in_Color_2;
  colors[3] = in_Color_3;
}
)";

const char *const SHADER_TEXT_FS = R"(
#version 330 core

layout(location = 0) out vec4 out_Color;

uniform sampler2D u_Texture;
uniform vec4 u_Palette[255];

in vec2 tex_Position;
in float tex_Index;

out vec4 colors[4];

void main() {
  int index = int(texture(u_Texture, tex_Position).r * 255.0f + 0.5);
  //out_Color = vec4(0, 0, 1, 1);
  if(index == 0)
  {
    discard;
  }
  else
    out_Color = u_Palette[index - 1];
  //out_Color = vec4(vec3(gl_FragCoord.z), 1.0);
}
)";

unsigned int link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader)
{
  unsigned int program = glCreateProgram();

  glAttachShader(program, vertex_Shader);
  glAttachShader(program, fragment_Shader);
  glLinkProgram(program);

  return program;
}

bool validate_Program(unsigned int program)
{
  glValidateProgram(program);

  int res;

  glGetProgramiv(program, GL_VALIDATE_STATUS, &res);
  if (res == GL_FALSE)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetProgramInfoLog(program, length, &length, message);
    gear::error("OpenGl program validation failed: %s", message);
    glDeleteShader(program);
    return 0;
  }
}

_GEAR_END