#pragma once

#include <gear/core/core.h>
#include <gear/data/FileStream.h>

_GEAR_START

class Resource {
public:
  static Ref<Resource> load_Resource(const char *filename);
protected:
  virtual int load(FileStream *file_Stream) = 0;
};

_GEAR_END