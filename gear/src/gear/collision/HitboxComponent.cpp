#include "HitboxComponent.h"
#include "HurtboxComponent.h"

#include <gear/scene/Entity.h>
#include <gear/scene/TransformComponent.h>
#include <vector>

#include <gear/core/debug/log.h>

static std::vector<std::function<void(void)>> callbacks;

void gear::hitbox_Collision_Check(gear::Scene *scene)
{
    size_t hitbox_Count, hurtbox_Count;
    int ints = 0;
    uint8_t scene_ID = scene->get_ID();
    Component<HitboxComponent> *hitbox_Comps = scene->get_All_Components<HitboxComponent>(&hitbox_Count);
    Component<HurtboxComponent> *hurtbox_Comps = scene->get_All_Components<HurtboxComponent>(&hurtbox_Count);
    Entity *hitbox_Entities = new Entity[hitbox_Count];
    Entity *hurtbox_Entities = new Entity[hurtbox_Count];

    Entity e1, e2;

    for (int i = 0; i < hitbox_Count; i++)
    {
        hitbox_Entities[i] = Entity{hitbox_Comps[i].get_Entity_ID(), scene_ID};
        hitbox_Entities[i].update_Transformation();
    }

    for (int i = 0; i < hurtbox_Count; i++)
    {
        hurtbox_Entities[i] = Entity{hurtbox_Comps[i].get_Entity_ID(), scene_ID};
        hurtbox_Entities[i].update_Transformation();
    }

    Vector<double, 2> vec;

    for (int i = 0; i < hitbox_Count; i++)
        for (int j = 0; j < hurtbox_Count; j++)
        {
            if (hitbox_Entities[i].get_Entity_ID() == hurtbox_Entities[j].get_Entity_ID())
                continue;
            for (auto &hitbox : hitbox_Comps[i].data.hitboxes)
            {
                auto layer = hitbox->get_Layer();
                for (auto &hurtbox : hurtbox_Comps[j].data.hurtboxes)
                {
                    if (layer != hurtbox->get_Layer())
                        continue;
                    bool previous_Intersection = hitbox->intersected(*hurtbox.get());
                    bool intersection = hitbox->intersects(*hurtbox.get());

                    if (intersection)
                    {
                        if (!previous_Intersection)
                            if (hitbox->m_On_Collision_Begin)
                                callbacks.push_back(std::bind(hitbox->m_On_Collision_Begin, HitboxEvent{hitbox_Entities[i], hurtbox_Entities[j], layer}));
                        if (hitbox->m_On_Colliding)
                            callbacks.push_back(std::bind(hitbox->m_On_Colliding, HitboxEvent{hitbox_Entities[i], hurtbox_Entities[j], layer}));
                    }
                    else if (previous_Intersection)
                        if (hitbox->m_On_Collision_End)
                            callbacks.push_back(std::bind(hitbox->m_On_Collision_End, HitboxEvent{hitbox_Entities[i], hurtbox_Entities[j], layer}));
                }
            }
        }

    for (int i = 0; i < hitbox_Count; i++)
        for (auto &hitbox : hitbox_Comps[i].data.hitboxes)
            hitbox->set_Previous();

    for (int i = 0; i < hurtbox_Count; i++)
        for (auto &hurtbox : hurtbox_Comps[i].data.hurtboxes)
            hurtbox->set_Previous();

    for (auto &callback : callbacks)
        callback();
    callbacks.clear();

    delete[] hitbox_Entities;
    delete[] hurtbox_Entities;
}