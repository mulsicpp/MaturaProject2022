#include "shaders.h"

_GEAR_START

char shader_Buffer[GEAR_MAX_SHADER_CODE_LENGTH];

const char *const SHADER_UPSCALE_VS = 
"#version 430 core\n"

"layout(location = 0) in vec2 in_Position;\n"
"layout(location = 1) in vec2 in_Tex_Position;\n"

"out vec2 tex_Position;\n"

"void main() {\n"
"  gl_Position = vec4(in_Position.x, in_Position.y, 0.0f, 1.0f);\n"
"  tex_Position = in_Tex_Position;\n"
"}\n";

const char *const SHADER_UPSCALE_FS = 
"#version 430 core\n"

"layout(location = 0) out vec4 out_Color;\n"

"layout(binding = 0) uniform sampler2D u_Texture;\n"

"in vec2 tex_Position;\n"

"void main() {\n"
"  out_Color = texture(u_Texture, tex_Position);\n"
"}\n";



const char *const SHADER_SPRITE_VS = 
"#version 430 core\n"

"layout(location = 0) in vec3 in_Position;\n"
"layout(location = 1) in vec2 in_Tex_Position;\n"
"layout(location = 2) in float in_Index;\n"

"uniform int u_Frame_Width;\n"
"uniform int u_Frame_Height;\n"

"out vec2 tex_Position;\n"
"out flat int tex_Index;\n"

"void main() {\n"
"  gl_Position = vec4((in_Position.x * 2.0f / float(u_Frame_Width)) - 1.0f, 1.0f - (in_Position.y * 2.0f / float(u_Frame_Height)), in_Position.z, 1.0f);\n"
"  tex_Position = in_Tex_Position;\n"
"  tex_Index = int(in_Index);\n"
"}\n"
;

const char *const SHADER_SPRITE_FS = 
"#version 430 core\n"

"layout(location = 0) out vec4 out_Color;\n"

"layout(binding = 0) uniform sampler2D u_Texture[%i];\n"
"layout(binding = 1) uniform sampler1D u_Palette[%i];\n"

"in vec2 tex_Position;\n"
"in flat int tex_Index;\n"

"void main() {\n"
"  int index = int(texture(u_Texture[tex_Index], tex_Position).r * 255.0f + 0.5);\n"
"  if(index == 0)\n"
"  {\n"
"    discard;\n"
"  }\n"
"  else\n"
"  out_Color = texture(u_Palette[tex_Index], (float(index) - 0.5f) / 255.0f);\n"
"  //out_Color = vec4(vec3(1 - gl_FragCoord.z), 1.0);\n"
"}\n"
;

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