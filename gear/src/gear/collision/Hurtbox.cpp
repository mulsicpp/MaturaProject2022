#include "Hurtbox.h"

uint32_t gear::Hurtbox::get_Layer(void) const 
{
    return m_Layer;
}


void gear::Hurtbox::set_Layer(uint32_t layer) 
{
    m_Layer = layer;
}