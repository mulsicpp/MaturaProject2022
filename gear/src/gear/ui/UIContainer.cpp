#include "UIContainer.h"
#include "UIFocusableComponent.h"

gear::UIContainer::UIContainer(uint8_t id)
{
    
}
gear::UIContainer::~UIContainer()
{
}

gear::Ref<gear::UIContainer> gear::UIContainer::create(uint8_t id)
{
    return Ref<UIContainer>(new UIContainer(id));
}

void gear::UIContainer::generate_Navigation(void)
{
    for (Ref<UIComponent> component1 : m_Components)
    {
        if (component1->is_Focusable())
        {
            UIFocusableComponent *fcomponent1 = (UIFocusableComponent *)component1.get();
            Vector<int, 2> gridpos1 = fcomponent1->get_Grid_Pos();
            fcomponent1->set_Navigation_Component(UIDirection::DOWN, nullptr);
            fcomponent1->set_Navigation_Component(UIDirection::LEFT, nullptr);
            fcomponent1->set_Navigation_Component(UIDirection::RIGHT, nullptr);
            fcomponent1->set_Navigation_Component(UIDirection::UP, nullptr);
            for (Ref<UIComponent> component2 : m_Components)
            {
                if (component2->is_Focusable())
                {
                    Ref<UIFocusableComponent> fcomponent2 = std::static_pointer_cast<UIFocusableComponent>(component2);
                    Vector<int, 2> gridpos2 = fcomponent2->get_Grid_Pos();
                    if (gridpos1[0] == gridpos2[0])
                    {
                        if (gridpos1[1] > gridpos2[1] && (fcomponent1->m_Navigation_Components[(int)UIDirection::UP] == nullptr || fcomponent1->m_Navigation_Components[(int)UIDirection::UP]->get_Grid_Pos()[1] < gridpos2[1]))
                        {
                            fcomponent1->m_Navigation_Components[(int)UIDirection::UP] = fcomponent2;
                        }
                        else if (gridpos1[1] < gridpos2[1] && (fcomponent1->m_Navigation_Components[(int)UIDirection::DOWN] == nullptr || fcomponent1->m_Navigation_Components[(int)UIDirection::DOWN]->get_Grid_Pos()[1] > gridpos2[1]))
                        {
                            fcomponent1->m_Navigation_Components[(int)UIDirection::DOWN] = fcomponent2;
                        }
                    }
                    else if (gridpos1[1] == gridpos2[1])
                    {
                        if (gridpos1[0] > gridpos2[0] && (fcomponent1->m_Navigation_Components[(int)UIDirection::LEFT] == nullptr || fcomponent1->m_Navigation_Components[(int)UIDirection::LEFT]->get_Grid_Pos()[0] < gridpos2[0]))
                        {
                            fcomponent1->m_Navigation_Components[(int)UIDirection::LEFT] = fcomponent2;
                        }
                        else if (gridpos1[0] < gridpos2[0] && (fcomponent1->m_Navigation_Components[(int)UIDirection::RIGHT] == nullptr || fcomponent1->m_Navigation_Components[(int)UIDirection::RIGHT]->get_Grid_Pos()[0] > gridpos2[0]))
                        {
                            fcomponent1->m_Navigation_Components[(int)UIDirection::RIGHT] = fcomponent2;
                        }
                    }
                }
            }
        }
    }
}

std::vector<gear::Ref<gear::UIComponent>> gear::UIContainer::get_Components(void) const {
    return m_Components;
}