#include <gear/gear.h>

#include <gear/resource/Palette.h>
#include <gear/renderer/Camera.h>
#include <gear/collision/shapes/Shape.h>
#include <gear/collision/Collider.h>

class MyCamera : public gear::Camera
{
public:
    MyCamera(const gear::Vector<double, 2> *target_Position);
    void follow_Target(void) override;
};

class MyGame : public gear::Game
{
private:
    gear::Entity p1, p2;
    gear::Entity health_P1, health_P2;
    gear::Ref<gear::Palette> palettes[7];
    int palette_Index = 0;
    gear::Vector<double, 2> m_Target_Pos;

    gear::Vector<double, 2> cam_Pos;

    double time = 0;

    MyCamera cam = &cam_Pos;
    double velocity = 1;

public:
    void on_Startup(void) override;
    void render(void) override;
    void on_Shutdown(void) override;
};