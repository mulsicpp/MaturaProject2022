#include "UIContainer.h"

gear::UIContainer::UIContainer(uint8_t id)
{
    m_Scene = Scene::get(id);
    m_Scene->create();
}
gear::UIContainer::~UIContainer()
{
    m_Scene->destroy();
}

gear::Ref<gear::UIContainer> gear::UIContainer::create(uint8_t id)
{
    return Ref<UIContainer>(new UIContainer(id));   
}