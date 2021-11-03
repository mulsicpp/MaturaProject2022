#include "log_functions.h"

static FILE *_p_File = nullptr;
static uint8_t _flags = 0;

FILE *gear::log_Get_File(void)
{
  return _p_File;
}

uint8_t gear::log_Get_Flags(void)
{
  return _flags;
}

void gear::log_Set_Output(uint8_t flags)
{
  _flags = flags;
}

void gear::log_Open_File(const char *filename)
{
  _p_File = fopen(filename, "w");
}

void gear::log_Close_File()
{
  fclose(_p_File);
}