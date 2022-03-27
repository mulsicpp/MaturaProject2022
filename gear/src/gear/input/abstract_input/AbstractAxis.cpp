#include "AbstractAxis.h"

gear::AbstractOrAxis::AbstractOrAxis(const std::vector<gear::Ref<gear::AbstractAxis>> &axes)
    : m_Axes(axes)
{
    for (const Ref<AbstractAxis> &axis : m_Axes)
    {
        axis->set_Callback([this](float value)
                           {
        float temp = this->get_Value();
        if(value > temp) {
            temp = value;
            if(m_Callback != nullptr)
                m_Callback(value);
        } });
    }
}

float gear::AbstractOrAxis::get_Value(void) const
{
    float max_Value = 0, value = 0;
    for (const Ref<AbstractAxis> &axis : m_Axes)
        if(abs(value = axis->get_Value()) > abs(max_Value))
            max_Value = value;
    return max_Value;
}