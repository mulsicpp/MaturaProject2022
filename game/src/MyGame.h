#include <gear/gear.h>

#include <gear/resource/Palette.h>
#include <gear/renderer/Camera.h>
#include <gear/collision/shapes/Shape.h>

class MyGame : public gear::Game
{
private:
  gear::Scene *m_Scene;
  gear::Entity *eis;
  gear::Entity *eis_Idle;
  gear::Ref<gear::Palette> palettes[7];
  int palette_Index = 0;

  gear::Ref<gear::Shape> s1;
  gear::Ref<gear::Shape> s2;

  gear::Vector<double, 2> cam_Pos;

  gear::Camera cam = &cam_Pos;
  double speed = 1;

public:
  void on_Startup(void) override;
  void per_Frame(void) override;
  void on_Shutdown(void) override;
};