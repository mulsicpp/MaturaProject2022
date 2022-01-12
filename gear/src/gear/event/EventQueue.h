#pragma once

#include <gear/core/core.h>
#include <mutex>
#include <vector>
#include <functional>

_GEAR_START

template <class T>
class EventQueue
{
    friend class T;

private:
    static std::vector<T> m_Queue;
    static std::vector<std::function<void(T)>> global_Event_Callbacks;
    static void push_Event(T event)
    {
        m_Queue.push_back(event);
    }
};

_GEAR_END