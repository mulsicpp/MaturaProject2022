#pragma once

#include <gear/core/core.h>
#include <gear/scene/Scene.h>
#include <vector>
#include "UIComponent.h"

_GEAR_START

class UIContainer
{
private:
    Scene *m_Scene;
    std::vector<Ref<UIComponent>> m_Components;
    UIContainer(uint8_t id);
    ~UIContainer();
    UIContainer(const UIContainer &container) = delete;
    UIContainer &operator=(const UIContainer &container) = delete;

public:
    static Ref<UIContainer> create(uint8_t id);
    template<class T>
    Ref<T> create_Component(void){
        return std::make_shared<T>();
    }
};

_GEAR_END