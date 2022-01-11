#include <gear/gear.h>

class MyGame : public gear::Game
{
  void on_Startup(void) override;
  void per_Frame(void) override;
  void on_Shutdown(void) override;
};