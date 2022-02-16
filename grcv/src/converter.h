#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <gear/data/FileStream.h>
#include <cstring>

std::vector<std::string> find_Convertables(const char* folder);

void cmd_To_Args(char *cmd, int max_Args, int *argc, char **argv);

class Converter
{
protected:
  FILE *m_File_In;
  gear::FileStream *m_File_Out;

  virtual void command(int argc, char **argv) = 0;
  virtual void convert(void) = 0;

public:
  Converter(FILE *file_In, gear::FileStream *file_Out);
  virtual void execute();
};

struct ConversionUnit {
  std::string source, target;
};