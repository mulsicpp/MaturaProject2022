#include "RegisteredInputs.h"

std::vector<gear::InputDevice> gear::RegisteredInputs::m_Registered_Inputs;

std::vector<gear::InputDevice> gear::RegisteredInputs::get_All(void){
    return m_Registered_Inputs;
}
gear::InputDevice gear::RegisteredInputs::get(int index){
    return m_Registered_Inputs.at(index);
}
void gear::RegisteredInputs::add(InputDevice input){
    m_Registered_Inputs.push_back(input);
}
void gear::RegisteredInputs::remove(InputDevice input){
    m_Registered_Inputs.erase(std::remove(m_Registered_Inputs.begin(), m_Registered_Inputs.end(), input), m_Registered_Inputs.end());
}
void gear::RegisteredInputs::remove(int index){
    m_Registered_Inputs.erase(m_Registered_Inputs.begin() + index);
}
void gear::RegisteredInputs::remove_All(void){
    m_Registered_Inputs.clear();
}