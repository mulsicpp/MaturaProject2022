#pragma once

#include <gear/core/core.h>
#include <mutex>
#include <vector>
#include <functional>
#include <gear/scene/Entity.h>
#include "EventComponent.h"

_GEAR_START

class Input;

template <class T>
class EventQueue
{
    friend class Input;

private:
    static std::vector<T> m_Queue;
    static std::vector<std::function<void(T)>> m_global_Event_Callbacks;

    static void event_Callback(EventComponent<T> &event_Component)
    {
        for (T &event : m_Queue)
        {
            if (event_Component.active)
                event_Component.callback(event);
        }
    }

    static void dispatch_Events(Scene *scene)
    {
        for (T &event : m_Queue)
        {
            for (auto &callback : m_global_Event_Callbacks)
            {
                callback(event);
            }
        }
        gear::Entity::for_Each<EventComponent<T>>(scene->get_ID(), event_Callback);
        m_Queue.clear();
    }

public:
    static void push_Event(T event)
    {
        GEAR_DEBUG_LOG("before push back");
        m_Queue.push_back(event);
        GEAR_DEBUG_LOG("after push back");
    }
};

template<class T>
std::vector<T> gear::EventQueue<T>::m_Queue;
template<class T>
std::vector<std::function<void(T)>> gear::EventQueue<T>::m_global_Event_Callbacks;

_GEAR_END