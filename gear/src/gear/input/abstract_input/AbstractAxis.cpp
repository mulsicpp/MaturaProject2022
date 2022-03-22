#include "AbstractAxis.h"

void gear::AbstractAxis::set_Callback(const std::function<void(float)> &callback)
{
  m_Callback = callback;
}

gear::AbstractOrAxis::AbstractOrAxis(const gear::Ref<gear::AbstractAxis> &first, const gear::Ref<gear::AbstractAxis> &second)
    : m_First(first), m_Second(second)
{
    m_First->set_Callback([this] (float value) {
        float temp = this->get_Value();
        if(value > temp) {
            temp = value;
            if(m_Callback != nullptr)
                m_Callback(value);
        }
    });

    m_Second->set_Callback([this] (float value) {
        float temp = this->get_Value();
        if(value > temp) {
            temp = value;
            if(m_Callback != nullptr)
                m_Callback(value);
        }
    });
}

float gear::AbstractOrAxis::get_Value(void) const {
    float first_Value = m_First->get_Value(), second_Value = m_Second->get_Value();
    return abs(first_Value) > abs(second_Value) ? first_Value : second_Value;
}