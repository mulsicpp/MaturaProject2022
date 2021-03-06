#include "DynamicPhysicsComponent.h"

#include <gear/scene/Entity.h>
#include <gear/scene/TransformComponent.h>

#include <gear/core/Game.h>

#include <gear/core/debug/log.h>

static std::vector<std::function<void(void)>> callbacks;

static double physics_Iterations_Per_Second = 180;

void gear::physics_Step(gear::Scene *scene)
{
    size_t dynamic_Count, static_Count;
    int ints = 0;
    uint8_t scene_ID = scene->get_ID();
    Component<DynamicPhysicsComponent> *dynamic_Phys = scene->get_All_Components<DynamicPhysicsComponent>(&dynamic_Count);
    Component<StaticPhysicsComponent> *static_Phys = scene->get_All_Components<StaticPhysicsComponent>(&static_Count);
    TransformComponent **transforms = new TransformComponent *[dynamic_Count];
    Entity *dynamic_Entities = new Entity[dynamic_Count];
    Entity *static_Entities = new Entity[static_Count];

    Entity e1, e2;

    for (int i = 0; i < dynamic_Count; i++)
    {
        dynamic_Entities[i] = Entity{dynamic_Phys[i].get_Entity_ID(), scene_ID};
        transforms[i] = dynamic_Entities[i].get<TransformComponent>();
        dynamic_Phys[i].data.velocity += dynamic_Phys[i].data.acceleration / physics_Iterations_Per_Second;
        if (dynamic_Phys[i].data.velocity[0] < dynamic_Phys[i].data.velocity_X_Interval[0])
            dynamic_Phys[i].data.velocity[0] = dynamic_Phys[i].data.velocity_X_Interval[0];

        if (dynamic_Phys[i].data.velocity[0] > dynamic_Phys[i].data.velocity_X_Interval[1])
            dynamic_Phys[i].data.velocity[0] = dynamic_Phys[i].data.velocity_X_Interval[1];

        if (dynamic_Phys[i].data.velocity[1] < dynamic_Phys[i].data.velocity_Y_Interval[0])
            dynamic_Phys[i].data.velocity[1] = dynamic_Phys[i].data.velocity_Y_Interval[0];

        if (dynamic_Phys[i].data.velocity[1] > dynamic_Phys[i].data.velocity_Y_Interval[1])
            dynamic_Phys[i].data.velocity[1] = dynamic_Phys[i].data.velocity_Y_Interval[1];

        transforms[i]->position += dynamic_Phys[i].data.velocity / physics_Iterations_Per_Second;

        dynamic_Entities[i].update_Transformation();
    }

    for (int i = 0; i < static_Count; i++)
    {
        static_Entities[i] = Entity{static_Phys[i].get_Entity_ID(), scene_ID};
    }

    Vector<double, 2> vec;

    for (int i = 0; i < dynamic_Count; i++)
    {
        for (int j = i + 1; j < dynamic_Count; j++)
        {
            bool previous_Intersection = dynamic_Phys[i].data.collider->intersected(*dynamic_Phys[j].data.collider);
            for (auto &hitbox1 : dynamic_Phys[i].data.collider->get_Shapes())
            {
                for (auto &hitbox2 : dynamic_Phys[j].data.collider->get_Shapes())
                {
                    if (hitbox1.absolute_Shape->intersects(hitbox2.absolute_Shape.get(), &vec))
                    {
                        if (dynamic_Phys[i].data.on_Overlap != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[i].data.on_Overlap, CollisionEvent{dynamic_Entities[i], dynamic_Entities[j], vec, previous_Intersection}));
                        if (dynamic_Phys[j].data.on_Overlap != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[j].data.on_Overlap, CollisionEvent{dynamic_Entities[j], dynamic_Entities[i], -vec, previous_Intersection}));
                        
                        if (!dynamic_Phys[i].data.check({dynamic_Entities[i], dynamic_Entities[j], vec, previous_Intersection}) || !dynamic_Phys[j].data.check({dynamic_Entities[j], dynamic_Entities[i], -vec, previous_Intersection}))
                            continue;

                        if (dynamic_Phys[i].data.on_Collision != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[i].data.on_Collision, CollisionEvent{dynamic_Entities[i], dynamic_Entities[j], vec, previous_Intersection}));
                        if (dynamic_Phys[j].data.on_Collision != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[j].data.on_Collision, CollisionEvent{dynamic_Entities[j], dynamic_Entities[i], -vec, previous_Intersection}));

                        double inv_Mass1, inv_Mass2;

                        inv_Mass1 = 1 / dynamic_Phys[i].data.mass;
                        inv_Mass2 = 1 / dynamic_Phys[j].data.mass;
                        double sum = dynamic_Phys[i].data.mass + dynamic_Phys[j].data.mass;
                        transforms[i]->position -= vec * dynamic_Phys[j].data.mass / sum;
                        transforms[j]->position += vec * dynamic_Phys[i].data.mass / sum;

                        dynamic_Entities[i].update_Transformation();
                        dynamic_Entities[j].update_Transformation();

                        Vector<double, 2> relative_Speed = dynamic_Phys[j].data.velocity - dynamic_Phys[i].data.velocity;
                        Vector<double, 2> normal = vec / vec.mag();
                        double velocity_Normal = relative_Speed * normal;

                        if (velocity_Normal > 0)
                            continue;

                        double e = dynamic_Phys[j].data.restitution < dynamic_Phys[i].data.restitution ? dynamic_Phys[j].data.restitution : dynamic_Phys[i].data.restitution;

                        double impulse_Scalar = (-(1 + e) * velocity_Normal) / (inv_Mass1 + inv_Mass2);

                        Vector<double, 2> impulse = normal * impulse_Scalar;

                        dynamic_Phys[i].data.velocity -= impulse * inv_Mass1;
                        dynamic_Phys[j].data.velocity += impulse * inv_Mass2;

                        if (dynamic_Phys[i].data.on_Collision_Resolved != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[i].data.on_Collision_Resolved, CollisionEvent{dynamic_Entities[i], dynamic_Entities[j], vec, previous_Intersection}));
                        if (dynamic_Phys[j].data.on_Collision_Resolved != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[j].data.on_Collision_Resolved, CollisionEvent{dynamic_Entities[j], dynamic_Entities[i], -vec, previous_Intersection}));
                    }
                }
            }
        }
    }

    for (int i = 0; i < static_Count; i++)
    {
        for (int j = 0; j < dynamic_Count; j++)
        {
            bool previous_Intersection = static_Phys[i].data.collider->intersected(*dynamic_Phys[j].data.collider);
            for (auto &hitbox1 : static_Phys[i].data.collider->get_Shapes())
            {
                for (auto &hitbox2 : dynamic_Phys[j].data.collider->get_Shapes())
                {
                    if (hitbox1.absolute_Shape->intersects(hitbox2.absolute_Shape.get(), &vec))
                    {
                        if (static_Phys[i].data.on_Overlap != nullptr)
                            callbacks.push_back(std::bind(static_Phys[i].data.on_Overlap, CollisionEvent{static_Entities[i], dynamic_Entities[j], vec, previous_Intersection}));
                        if (dynamic_Phys[j].data.on_Overlap != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[j].data.on_Overlap, CollisionEvent{dynamic_Entities[j], static_Entities[i], -vec, previous_Intersection}));

                        if (!static_Phys[i].data.check({static_Entities[i], dynamic_Entities[j], vec, previous_Intersection}) || !dynamic_Phys[j].data.check({dynamic_Entities[j], static_Entities[i], -vec, previous_Intersection}))
                            continue;

                        if (static_Phys[i].data.on_Collision != nullptr)
                            callbacks.push_back(std::bind(static_Phys[i].data.on_Collision, CollisionEvent{static_Entities[i], dynamic_Entities[j], vec, previous_Intersection}));
                        if (dynamic_Phys[j].data.on_Collision != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[j].data.on_Collision, CollisionEvent{dynamic_Entities[j], static_Entities[i], -vec, previous_Intersection}));

                        double inv_Mass;

                        inv_Mass = 1 / dynamic_Phys[j].data.mass;
                        transforms[j]->position += vec;

                        dynamic_Entities[j].update_Transformation();

                        Vector<double, 2> normal = vec / vec.mag();
                        double velocity_Normal = dynamic_Phys[j].data.velocity * normal;

                        if (velocity_Normal > 0)
                            continue;

                        double e = dynamic_Phys[j].data.restitution < static_Phys[i].data.restitution ? dynamic_Phys[j].data.restitution : static_Phys[i].data.restitution;

                        double impulse_Scalar = (-(1 + e) * velocity_Normal) / (inv_Mass);

                        Vector<double, 2> impulse = normal * impulse_Scalar;

                        dynamic_Phys[j].data.velocity += impulse * inv_Mass;

                        if (static_Phys[i].data.on_Collision_Resolved != nullptr)
                            callbacks.push_back(std::bind(static_Phys[i].data.on_Collision_Resolved, CollisionEvent{static_Entities[i], dynamic_Entities[j], vec, previous_Intersection}));
                        if (dynamic_Phys[j].data.on_Collision_Resolved != nullptr)
                            callbacks.push_back(std::bind(dynamic_Phys[j].data.on_Collision_Resolved, CollisionEvent{dynamic_Entities[j], static_Entities[i], -vec, previous_Intersection}));
                    }
                }
            }
        }
    }

    for (int i = 0; i < dynamic_Count; i++)
        dynamic_Phys[i].data.collider->set_Previous();
    for (int i = 0; i < static_Count; i++)
        static_Phys[i].data.collider->set_Previous();

    for (auto &callback : callbacks)
        callback();
    callbacks.clear();

    delete[] transforms;
    delete[] dynamic_Entities;
    delete[] static_Entities;
    // GEAR_DEBUG_LOG("collisions: %i", ints);
}

static double time_Passed = 0;

void gear::physics_Timed_Step(gear::Scene *scene)
{
    double new_Time = time_Passed + gear::Game::get_Delta_Time();
    int count = int(new_Time * physics_Iterations_Per_Second) - int(time_Passed * physics_Iterations_Per_Second);
    for (int i = 0; i < count; i++)
        physics_Step(scene);
    time_Passed = new_Time;
}