#include <gear/gear.h>

#include <gear/resource/Palette.h>

class MyGame : public gear::Game
{
private:
  gear::Scene *m_Scene;
  gear::Entity *eis;
  gear::Ref<gear::Palette> palettes[3];
  int palette_Index = 0;

public:
  void on_Startup(void) override;
  void per_Frame(void) override;
  void on_Shutdown(void) override;
};