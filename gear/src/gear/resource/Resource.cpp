#include "Resource.h"
#include "Font.h"
#include "Palette.h"

gear::Ref<gear::Resource> gear::Resource::load_Resource(const char *filename) {
  gear::FileStream* file_Stream = gear::FileStream::open(filename, "rb");
  char signature[7];
  file_Stream->get<char>(signature, 7);
  Ref<Resource> ret;
  if(strcmp(signature, "GEARFNT") == 0)
    ret = std::make_shared<Font>();
  else
    return nullptr;
  ret->load(file_Stream);
  return ret;
}