#pragma once

#include <gear/core/core.h>
#include <mutex>
#include <vector>

_GEAR_START

template <class T>
class EventQueue
{
private:
    static std::mutex m_Mutex;
    static std::vector<T> m_Queue;
};


_GEAR_END