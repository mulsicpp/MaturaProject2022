#include <gear/scripting/ScriptableEntity.h>
#include <gear/collision/Hitbox.h>

class EisScript : public gear::ScriptableEntity
{
    int jumps = 3;
    bool ground = false;
    gear::Ref<gear::Hitbox> attack;

    virtual void on_Create(void) override;
    virtual void on_Update(void) override;
    virtual void on_Destroy(void) override;
};