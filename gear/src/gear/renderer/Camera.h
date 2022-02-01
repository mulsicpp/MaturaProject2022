#pragma once

#include <gear/core/core.h>
#include <gear/math/Vector.h>

_GEAR_START

class Camera
{
protected:
  Vector<float, 2> m_Position;
  const Vector<float, 2> *m_Target_Position;

public:
  Camera(const Vector<float, 2> *target_Position);
  virtual void follow_Target(void);

  const Vector<float, 2> *get_Target_Position(void);
  Vector<float, 2> get_Position(void);
};

_GEAR_END