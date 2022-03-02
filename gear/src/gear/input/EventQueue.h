#pragma once

#include <gear/core/core.h>
#include <mutex>
#include <vector>
#include <functional>
#include <gear/scene/Entity.h>
#include "InputComponent.h"
#include <mutex>
#include <gear/core/debug/log.h>
#include <unordered_map>
#include "abstract_input/EventHandler.h"

_GEAR_START

class Input;

template <class T>
class EventQueue
{
  friend class Input;
  friend class EventHandler<T>;

private:
  static std::vector<T> m_Queue;
  static std::vector<std::function<void(T)>> m_Global_Event_Callbacks;
  static std::unordered_map<uint64_t, EventHandler<T> *> m_Event_Handlers;
  static std::mutex m_Mutex;

  static void event_Callback(InputComponent<T> &event_Component)
  {
    for (T &event : m_Queue)
    {
      if (event_Component.active)
        event_Component.callback(event);
    }
  }

  static void dispatch_Events(Scene *scene)
  {
    m_Mutex.lock();
    for (T &event : m_Queue)
    {
      for (auto &callback : m_Global_Event_Callbacks)
      {
        callback(event);
      }
      for (std::pair<uint64_t, EventHandler<T> *> el : m_Event_Handlers)
      {
        el.second->handle_Event(event);
      }
    }

    if (scene != nullptr)
      gear::Entity::for_Each<InputComponent<T>>(scene->get_ID(), event_Callback);
    m_Queue.clear();
    m_Mutex.unlock();
  }

public:
  static void push_Event(T event)
  {
    m_Mutex.lock();
    m_Queue.push_back(event);
    m_Mutex.unlock();
  }
};

template <class T>
std::vector<T> gear::EventQueue<T>::m_Queue;
template <class T>
std::vector<std::function<void(T)>> gear::EventQueue<T>::m_Global_Event_Callbacks;
template <class T>
std::unordered_map<uint64_t, EventHandler<T> *> gear::EventQueue<T>::m_Event_Handlers;
template <class T>
std::mutex gear::EventQueue<T>::m_Mutex;

_GEAR_END