#pragma once
#include <vector>
#include <string>

std::vector<std::string> find_Convertables(const char* folder);


class Converter
{
private:
  FILE *m_File_In, *m_File_Out;
protected:
  virtual void command(int argc, char **argv) = 0;

public:
  Converter(FILE *file);
  virtual void execute();
  virtual void convert(void) = 0;
};

class ConversionUnit {

};