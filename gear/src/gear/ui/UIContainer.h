#pragma once

#include <gear/core/core.h>
#include <gear/scene/Scene.h>
#include <vector>
#include "UIComponent.h"

_GEAR_START

class SpritePipeline;

class UIContainer
{
    friend class SpritePipeline;
private:
    std::vector<Ref<UIComponent>> m_Components;
    UIContainer(uint8_t id);
    UIContainer(const UIContainer &container) = delete;
    UIContainer &operator=(const UIContainer &container) = delete;

public:
    ~UIContainer();
    static Ref<UIContainer> create(uint8_t id);
    void generate_Navigation(void);

    std::vector<Ref<UIComponent>> get_Components(void) const;

    template <class T>
    void add_Component(Ref<T> component)
    {
        m_Components.push_back(component);
    }
};

_GEAR_END