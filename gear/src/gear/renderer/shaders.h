#pragma once

#include <gear/core/core.h>
#include <glad/glad.h>

#define GEAR_MAX_SHADER_CODE_LENGTH 32000

_GEAR_START

extern char shader_Buffer[GEAR_MAX_SHADER_CODE_LENGTH];

extern const char *const SHADER_UPSCALE_VS;
extern const char *const SHADER_UPSCALE_FS;

extern const char *const SHADER_SPRITE_VS;
extern const char *const SHADER_SPRITE_FS;

extern const char *const SHADER_SHAPE_VS;
extern const char *const SHADER_SHAPE_FS;

//extern const char *SHADER_TEXT_VS;
//extern const char *SHADER_TEXT_FS;

template <class... Ts>
unsigned int get_Shader(const char *shader_Code, unsigned int type, Ts... params)
{
  sprintf(shader_Buffer, shader_Code, params...);

  unsigned int id = glCreateShader(type);
  const char *src = shader_Buffer;
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int res = 0;

  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  if (res == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char *)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    gear::error("Shader compilation failed: %s", message);
    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int link_Program(unsigned int vertex_Shader, unsigned int fragment_Shader);

bool validate_Program(unsigned int program);

_GEAR_END