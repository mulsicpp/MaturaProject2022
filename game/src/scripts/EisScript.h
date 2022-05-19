#include "BaseFighterScript.h"

class EisScript : public BaseFighterScript
{
protected:
    gear::AnimationComponent a_Projectile[2];
public:
    EisScript(gear::InputDevice device, std::string palette_Name);

    virtual void init_Animation_Events(void) override;

    void spawn_Projectile(void);
};