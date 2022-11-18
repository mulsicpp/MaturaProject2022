#include "BaseFighterScript.h"

class EisScript : public BaseFighterScript
{
protected:
    gear::AnimationComponent a_Projectile[2];

    //gear::
public:
    EisScript(gear::InputDevice device, std::string palette_Name, int player_Number);

    virtual void init_Animation_Events(void) override;
    virtual void init(void) override;

    void spawn_Projectile(void);
};