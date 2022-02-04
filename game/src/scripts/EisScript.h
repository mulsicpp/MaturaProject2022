#include <gear/scripting/ScriptableEntity.h>

class EisScript : public gear::ScriptableEntity {
  virtual void on_Create(void) override;
  virtual void on_Update(void) override;
  virtual void on_Destroy(void) override;
};