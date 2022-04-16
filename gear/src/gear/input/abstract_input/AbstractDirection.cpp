#include "AbstractDirection.h"

bool gear::AbstractDirection::is_Normalized(void)
{
    return m_Normalized;
}
void gear::AbstractDirection::set_Normalized(bool normalized)
{
    m_Normalized = normalized;
}