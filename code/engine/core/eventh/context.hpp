#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <engine/core/eventh/cancel.hpp>

using PollerFn = std::function<void()>;

// TODO: Event cancelation.

namespace eventh {
  
  unsigned int nextid = 0;

  struct EventhContext {
    EventhContext() {
      id = nextid++;
    }
    
    unsigned int id;
    
    template<typename Event>
    std::vector<Event>& queue() {
     static std::vector<std::vector<Event>> queues;
      if (queues.size() <= id) queues.resize(id + 1);
      return queues[id];
    }

   template<typename Event>
    std::vector<void(*)(Event&)>& listeners() {
      static std::vector<std::vector<void(*)(Event&)>> listeners;
      if (listeners.size() <= id) listeners.resize(id + 1);
      return listeners[id];
    }

    template<typename Event>
    inline void subscribe(void(*listener)(Event&)) {
      listeners<Event>().emplace_back(listener);
    }
    // * emit nincsen itt mivel circular dependency

  };

  std::vector<PollerFn>& pollers() {
    static std::vector<PollerFn> pollers;
    return pollers;
  }
}