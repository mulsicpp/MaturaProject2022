#include <gear/gear.h>

#include <gear/resource/Palette.h>
#include <gear/renderer/Camera.h>
#include <gear/collision/shapes/Shape.h>
#include <gear/collision/Hitbox.h>

class MyCamera : public gear::Camera {
public:
  MyCamera(const gear::Vector<double, 2> *target_Position);
  void follow_Target(void) override;
};

class MyGame : public gear::Game
{
private:
  gear::Scene *m_Scene;
  gear::Entity *eis;
  gear::Entity *eis_Idle;
  gear::Ref<gear::Palette> palettes[7];
  int palette_Index = 0;
  gear::Vector<double, 2> m_Target_Pos;

  gear::Hitbox h;

  gear::Vector<double, 2> cam_Pos;

  MyCamera cam = &cam_Pos;
  double velocity = 1;

public:
  void on_Startup(void) override;
  void per_Frame(void) override;
  void on_Shutdown(void) override;
};