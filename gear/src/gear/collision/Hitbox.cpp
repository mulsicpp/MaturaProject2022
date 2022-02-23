#include "Hitbox.h"

void gear::Hitbox::transform(gear::TransformComponent *transform) {
  m_Shape->transform(transform, m_Absolute_Shape.get());
}