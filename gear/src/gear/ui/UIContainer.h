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
    UIContainer(void);
    UIContainer(const UIContainer &container) = delete;
    UIContainer &operator=(const UIContainer &container) = delete;

public:
    ~UIContainer();
    static Ref<UIContainer> create(void);
    void generate_Navigation(void);

    std::vector<Ref<UIComponent>> get_Components(void) const;

    template <class T>
    void add_Component(T component)
    {
        Ref<UIComponent> comp = Ref<T>(new T(component));
        m_Components.push_back(comp);
    }
};

_GEAR_END