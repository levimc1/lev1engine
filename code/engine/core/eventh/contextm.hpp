#pragma once

#include <engine/core/eventh/context.hpp>
#include <vector>

// TODO: overoptimize


// TODO: use these more.
template<typename Event>
using Queue = std::vector<Event>;

template<typename Event>
using Listeners = std::vector<std::function<void(Event&)>>;


namespace eventh {

  namespace contextm {
    enum CarryState {
      LISTENERS_BIT = 1 << 0,
      EVENTS_BIT = 1 << 1
    };

    static std::vector<EventhContext> ctx_active; // az csak egy int...
    static std::unordered_map<unsigned int, int> ctx_carry; 
    
    template<typename Event>
    void all_listeners(std::vector<std::function<void(Event&)>>& out) {
      for (EventhContext& ctx : ctx_active) {
        if (ctx_carry[ctx.id] & LISTENERS_BIT) {
          for (auto& element : ctx.listeners<Event>()) {
            out.push_back(element);
          }
        }
      }
    }

    template<typename Event>
    void all_queues(std::vector<Event>& out) {
      for (EventhContext& ctx : ctx_active) {
        if (ctx_carry[ctx.id] & EVENTS_BIT) {
          for (auto& element : ctx.queue<Event>()) {
            out.push_back(element);
          }
        }
      }
    }
  }

  template<typename Event>
  void ensure_registered() {
    static bool registered = false;
    pollers().push_back([]() {
    Queue<Event> q; contextm::all_queues<Event>(q);
    Listeners<Event> l; contextm::all_listeners<Event>(l);

    for (auto& event : q) {
      for (auto& listener : l) {
        if (cancelled<Event>()) {break;}
        listener(event);
      }
    }
    q.clear();
    });
    registered = true;
  }
   
  static void use(const EventhContext& ctx) {
    contextm::ctx_active.clear();
    contextm::ctx_active.push_back(ctx); //! Main, azaz 0-dik
    contextm::ctx_carry.clear();
    using namespace contextm;
    contextm::ctx_carry.insert_or_assign(ctx.id, CarryState::EVENTS_BIT | CarryState::LISTENERS_BIT);
  }

  static void carry(const EventhContext& ctx, int state) {
    contextm::ctx_carry.insert_or_assign(ctx.id, state);
    // TODO: erase hozzáadása ha üres.
  }

  EventhContext init() {
    EventhContext ctx = EventhContext();
    use(ctx);
    return ctx;
  }
}