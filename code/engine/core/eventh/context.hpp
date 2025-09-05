#pragma once

#include <vector>
#include <unordered_map>
#include <functional>


using PollerFn = std::function<void()>;

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
      if (queues.size() <= id) queues.resize(id + 1);
      return queues[id];
    }

   template<typename Event>
    std::vector<void(*)(Event&)>& listeners() {
      static std::vector<std::vector<void(*)(Event&)>> listeners;
     if (listeners.size() <= id) listeners.resize(id + 1);
      return listeners[id];
    }

    std::vector<PollerFn>& pollers() {
      static std::vector<std::vector<PollerFn>> pollers;
      if (pollers.size() <= id) pollers.resize(id + 1);
      return pollers[id];
    }

    template<typename Event>
    void ensure_registered() {
      static std::unordered_map<unsigned int, bool> registereds;
      if (registereds.find(id) == registereds.end()) {
        pollers().push_back([this]() {
        auto& q = this->queue<Event>();
        auto& l = this->listeners<Event>();

        for (auto& event : q) {
          for (auto& listener : l) {
            listener(event);
          }
        }
        q.clear();
      });
      registereds[id] = true;
      }
    } 
  };
}