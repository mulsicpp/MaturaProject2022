#pragma once

#include "../core.h"
#include <stdint.h>
#include <time.h>

#define GEAR_CONSOLE 0x1
#define GEAR_FILE 0x2

#define GEAR_LOG_ENTRY(file, ...) \
fprintf(file, "[%04d-%02d-%02d %02d:%02d:%02d] ", timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);\
fprintf(file, __VA_ARGS__);\
fprintf(file, "\n")



_GEAR_START

FILE* log_Get_File(void);

uint8_t log_Get_Flags(void);

template <class... T>
void log(const char *str, T... args)
{
  FILE *_p_File = log_Get_File();
  uint8_t _flags = log_Get_Flags();
  time_t now;
  time(&now);
  tm * timeinfo = localtime(&now);
  if (_flags & GEAR_CONSOLE){
    GEAR_LOG_ENTRY(stdout, str, args...);
  }
  if (_p_File && _flags & GEAR_FILE)
  {
    GEAR_LOG_ENTRY(_p_File, str, args...);
  }
}

void log_Set_Output(uint8_t flags);

void log_Open_File(const char *filename);

void log_Close_File(void);

_GEAR_END