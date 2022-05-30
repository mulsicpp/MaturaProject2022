#pragma once

#include <gear/core/core.h>
#include <gear/resource/Font.h>
#include <gear/math/Vector.h>

#include <vector>
#include <string>

#include <gear/collision/shapes/Rect.h>

#include <gear/scene/Entity.h>

_GEAR_START

void _clear_Text_Cache(uint8_t scene_ID, unsigned int entityID);

struct TextComponent {
  std::string text;
  bool raw_Text = true;
  Ref<Font> font;
  Vector<double, 3> offset = {0, 0, 0};
  unsigned int width, height;
  std::vector<gear::Vector<uint8_t, 4>> colors;
  bool break_Word = false;
};

template<>
void gear::Component<gear::TextComponent>::on_Component_Remove(Entity entity, TextComponent *component)
{
  _clear_Text_Cache(entity.get_Scene_ID(), entity.get_Entity_ID());
  GEAR_DEBUG_LOG("cleared text cache %i", entity.get_Entity_ID());
}

_GEAR_END