#include <stdio.h>
#include <stdlib.h>
#include <filesystem>

#include "global.h"
#include "converter.h"
#include "font_converter.h"
#include "image_converter.h"
#include "animation_converter.h"
#include "palette_converter.h"

static std::vector<ConversionUnit> _conversion_Units;

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    error("program has to called like: grcv <source> <target>");
  }
  std::filesystem::path original_Path = std::filesystem::canonical(".");

  std::filesystem::path source = argv[1], target = argv[2];
  if (std::filesystem::is_directory(source))
  {
    target = std::filesystem::canonical(target);
    std::filesystem::current_path(source);
    std::filesystem::recursive_directory_iterator iterator(".");
    for (auto entry : iterator)
    {
      if (entry.path().extension().string() == ".grcv")
      {
        _conversion_Units.push_back({std::filesystem::proximate(std::filesystem::canonical(entry.path()), original_Path).string(),
                                     std::filesystem::proximate(target.string() + entry.path().parent_path().string().substr(1) + "/" + entry.path().stem().string() + ".gear", original_Path).string()});
      }
    }
  }
  else
  {
    _conversion_Units.push_back({source.string(), target.string()});
  }

  Converter *converter;
  for (ConversionUnit &unit : _conversion_Units)
  {
    std::filesystem::current_path(original_Path);
    FILE *file_In = fopen(unit.source.c_str(), "r");
    std::filesystem::create_directories(std::filesystem::path(target).parent_path());
    char buffer[512]{0};
    int argc = 0;
    char *argv[20];
    fgets(buffer, 512, file_In);

    cmd_To_Args(buffer, 20, &argc, argv);

    bool force = true;
    bool needs_Update = false;
    if (std::filesystem::exists(unit.target))
    {
      std::filesystem::file_time_type target_Time = std::filesystem::last_write_time(unit.target);
      if (target_Time < std::filesystem::last_write_time(unit.source)){

        printf("needs update because GRCV file %i %i\n", std::filesystem::last_write_time(unit.source), target_Time);
        needs_Update = true;
      }

      std::filesystem::current_path(std::filesystem::path(unit.source).parent_path());
      for (int i = 1; i < argc; i++)
        if (target_Time < std::filesystem::last_write_time(argv[i])){
          printf("needs update dependency\n");
          needs_Update = true;
        }
      std::filesystem::current_path(original_Path);
    }
    else
    {
      std::filesystem::create_directories(std::filesystem::path(unit.target).parent_path());
      needs_Update = true;
      printf("needs update because exists\n");
    }

    if (needs_Update)
    {
      if (strcmp("font", argv[0]) == 0)
      {
        printf("converting font \'%s\' to \'%s\' ...\n", unit.source.c_str(), unit.target.c_str());
        gear::FileStream *file_Out = gear::FileStream::open(unit.target.c_str(), "wb");
        file_Out->puts("GEARFNT");
        std::filesystem::current_path(std::filesystem::path(unit.source).parent_path());
        converter = new FontConverter(file_In, file_Out);
        converter->execute();
        delete converter;
        gear::FileStream::close(file_Out);
      }
      else if (strcmp("image", argv[0]) == 0)
      {
        printf("converting image \'%s\' to \'%s\' ...\n", unit.source.c_str(), unit.target.c_str());
        gear::FileStream *file_Out = gear::FileStream::open(unit.target.c_str(), "wb");
        file_Out->puts("GEARIMG");
        std::filesystem::current_path(std::filesystem::path(unit.source).parent_path());
        converter = new ImageConverter(file_In, file_Out);
        converter->execute();
        delete converter;
        gear::FileStream::close(file_Out);
      }
      else if (strcmp("animation", argv[0]) == 0)
      {
        printf("converting animation \'%s\' to \'%s\' ...\n", unit.source.c_str(), unit.target.c_str());
        gear::FileStream *file_Out = gear::FileStream::open(unit.target.c_str(), "wb");
        file_Out->puts("GEARANM");
        std::filesystem::current_path(std::filesystem::path(unit.source).parent_path());
        converter = new AnimationConverter(file_In, file_Out);
        converter->execute();
        delete converter;
        gear::FileStream::close(file_Out);
      }
      else if (strcmp("palette", argv[0]) == 0)
      {
        printf("converting palette \'%s\' to \'%s\' ...\n", unit.source.c_str(), unit.target.c_str());
        gear::FileStream *file_Out = gear::FileStream::open(unit.target.c_str(), "wb");
        file_Out->puts("GEARPLT");
        std::filesystem::current_path(std::filesystem::path(unit.source).parent_path());
        converter = new PaletteConverter(file_In, file_Out);
        converter->execute();
        delete converter;
        gear::FileStream::close(file_Out);
      }
    }

    fclose(file_In);
  }
  return 0;
}