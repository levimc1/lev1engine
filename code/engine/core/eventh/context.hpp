#pragma once

#include <vector>

using PollerFn = void(*)();

namespace eventh {
  
  unsigned int nextid;

  struct EventhContext {
    EventhContext() {
      id = nextid++;
    }
    unsigned int id;  

    template<typename Event>
    std::vector<Event>& queue() {
      static std::vector<std::vector<Event>> queues;
      return queues[id];
    }
    
    std::vector<PollerFn>& pollers() {
      static std::vector<std::vector<PollerFn>> pollers;
      return pollers[id];
    } 

    // TODO: replace with function wrapper!
    template<typename Event>
    std::vector<void(*)(Event&)>& listeners() {
      static std::vector<std::vector<void(*)(Event&)>> listeners;
      return listeners[id];
    }
    
    template<typename Event>
    void ensure_registered() {
      static std::unordered_map<unsignd int, bool> registereds;
      if (registerds.find(id) == register.end()) {
        pollers().push_back(&flush<Event>);
        registereds.emplace(id, true);       
      }

    }
  };
}