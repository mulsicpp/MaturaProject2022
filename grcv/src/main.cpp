#include <stdio.h>
#include "converter.h"
#include "global.h"
#include <filesystem>

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    error("program has to called like: grcv <source> <target>");
  }
  Converter *converter;
  std::filesystem::path source = std::filesystem::canonical(argv[1]), target = std::filesystem::canonical(argv[2]);
  std::filesystem::current_path(source);
  std::vector<std::string> convertables;
  if()
  return 0;
}