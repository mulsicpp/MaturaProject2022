#include <gear/gear.h>

class MyGame : public gear::Game
{
private:
  gear::Scene *m_Scene;

public:
  void on_Startup(void) override;
  void per_Frame(void) override;
  void on_Shutdown(void) override;
};