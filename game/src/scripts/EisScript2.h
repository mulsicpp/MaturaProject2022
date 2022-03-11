#include <gear/scripting/ScriptableEntity.h>

class EisScript2 : public gear::ScriptableEntity {
public:
  int jumps = 1;
  bool ground = false;

  virtual void on_Create(void) override;
  virtual void on_Update(void) override;
  virtual void on_Destroy(void) override;
};