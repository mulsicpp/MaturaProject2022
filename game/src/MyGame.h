#include <gear/gear.h>

#include <gear/resource/Palette.h>
#include <gear/renderer/Camera.h>

class MyGame : public gear::Game
{
private:
  gear::Scene *m_Scene;
  gear::Entity *eis;
  gear::Entity *eis_Idle;
  gear::Ref<gear::Palette> palettes[7];
  int palette_Index = 0;

  gear::Vector<float, 2> cam_Pos;

  gear::Camera cam = &cam_Pos;
  float speed = 1;

public:
  void on_Startup(void) override;
  void per_Frame(void) override;
  void on_Shutdown(void) override;
};