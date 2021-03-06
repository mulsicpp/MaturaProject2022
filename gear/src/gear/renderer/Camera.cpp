#include "Camera.h"

gear::Camera::Camera(const Vector<double, 2> *target_Position) : m_Position(*target_Position), m_Target_Position(target_Position) {}

void gear::Camera::follow_Target(void)
{
  m_Position = *m_Target_Position;
}

const gear::Vector<double, 2> *gear::Camera::get_Target_Position(void)
{
  return m_Target_Position;
}

gear::Vector<double, 2> gear::Camera::get_Position(void)
{
  return m_Position;
}