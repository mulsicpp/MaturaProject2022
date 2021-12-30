#include "converter.h"
#include <string.h>
#include <gear/data/memcpy.h>

void cmd_To_Args(char *cmd, int max_Args, int *argc, char **argv)
{
  int str_Length = strlen(cmd);

  bool ignore = false;

  for (int i = 0; i < str_Length; i++)
  {
    if (!ignore && (cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\t' || cmd[i] == '\r'))
      cmd[i] = 0;
    else if (cmd[i] == '\"')
    {
      ignore = !ignore;
      cmd[i] = 0;
    }
    else if (cmd[i] == '\\')
    {
      gear::memcpy(cmd + i, cmd + i + 1, str_Length - i);
    }
  }
  *argc = 0;

  if (str_Length > 0 && cmd[0] != 0)
  {
    argv[0] = cmd;
    (*argc)++;
  }

  for (int i = 1; i < str_Length && *argc < max_Args; i++)
  {
    if (cmd[i] != 0 && cmd[i - 1] == 0)
    {
      argv[*argc] = cmd + i;
      (*argc)++;
    }
  }
}

Converter::Converter(FILE *file_In, gear::FileStream *file_Out) : m_File_In(file_In), m_File_Out(file_Out) {}

void Converter::execute(void)
{
  char buffer[512];
  char *argv[256];
  int argc = 0;
  int str_Length = 0;
  while (fgets(buffer, 512, m_File_In))
  {
    cmd_To_Args(buffer, 256, &argc, argv);
    command(argc, argv);
  }
  convert();
}