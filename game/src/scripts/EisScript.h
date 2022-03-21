#include <gear/scripting/ScriptableEntity.h>
#include <gear/collision/Hitbox.h>

#include <gear/input/abstract_input/abstract_input.h>
#include <gear/input/abstract_input/AbstractButton.h>

GEAR_ABSTRACT_INPUT(FighterInput, KFighterInput, CFighterInput, GEAR_BUTTONS(left, right, up, down, attack, flash));

class EisScript : public gear::ScriptableEntity
{
    int jumps = 3;
    bool ground = false;
    gear::Ref<gear::Hitbox> attack;

    gear::Ref<FighterInput> m_Input;
public:
    EisScript() = default;
    EisScript(gear::InputDevice device);

    virtual void on_Create(void) override;
    virtual void on_Update(void) override;
    virtual void on_Destroy(void) override;
};