#pragma once

#include <gear/core/core.h>
#include "../EventQueue.h"

_GEAR_START

template<class T>
class EventHandler {
private:
  uint64_t m_ID;
  static uint64_t m_Next_ID;
public:
  EventHandler(void) {
    m_ID = m_Next_ID++;
    EventQueue<T>::m_Event_Handlers[m_ID] = this;
  }

  ~EventHandler() {
    EventQueue<T>::m_Event_Handlers.erase(m_ID);
  }

  virtual void handle_Event(T event) = 0;
};

template<class T>
uint64_t EventHandler<T>::m_Next_ID = 0;

_GEAR_END