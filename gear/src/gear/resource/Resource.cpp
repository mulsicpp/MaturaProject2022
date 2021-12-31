#include "Resource.h"
#include "Font.h"
#include "Palette.h"
#include "Sprite.h"
#include "Animation.h"

gear::Ref<gear::Resource> gear::Resource::load_Resource(const char *filename) {
  gear::FileStream* file_Stream = gear::FileStream::open(filename, "rb");
  char signature[7];
  file_Stream->get<char>(signature, 7);
  Ref<Resource> ret;
  if(strcmp(signature, "GEARFNT") == 0)
    ret = std::make_shared<Font>();
  else if(strcmp(signature, "GEARPLT") == 0)
    ret = std::make_shared<Palette>();
  else if(strcmp(signature, "GEARSPT") == 0)
    ret = std::make_shared<Sprite>();
  else if(strcmp(signature, "GEARANM") == 0)
    ret = std::make_shared<Animation>();
  else
    return nullptr;
  ret->load(file_Stream);
  return ret;
}