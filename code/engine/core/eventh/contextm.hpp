#pragma once

#include <engine/core/eventh/context.hpp>
#include <vector>

// TODO: overoptimize


// TODO: use these more.
template<typename Event>
using Queue = std::vector<Event>;

template<typename Event>
using Listeners = std::vector<std::function<void(Event&)>>;

template<typename Event>
using ListenerList = std::vector<std::vector<std::function<void(Event&)>>*>;

template<typename Event>
using QueueList = std::vector<std::vector<Event>*>;

namespace eventh {

  namespace contextm {
    enum CarryState {
      LISTENERS_BIT = 1 << 0,
      EVENTS_BIT = 1 << 1
    };

    static std::vector<EventhContext> ctx_active; // az csak egy int...
    static std::unordered_map<unsigned int, int> ctx_carry; 
  }

  namespace internal {
    
    using namespace eventh::contextm;
    template<typename Event>
    ListenerList<Event> all_listeners() {
      ListenerList<Event> out;
      for (EventhContext& ctx : ctx_active) {
        if (ctx_carry[ctx.id] & LISTENERS_BIT) {
          out.push_back(&ctx.listeners<Event>());
        }
      }
      return out;
    }

    template<typename Event>
    QueueList<Event> all_queues() {
      QueueList<Event> out;
      for (EventhContext& ctx : ctx_active) {
        if (ctx_carry[ctx.id] & EVENTS_BIT) {
          out.push_back(&ctx.queue<Event>());
        }
      }
      return out;
    }

    template<typename Event>
    void ensure_registered() {
      static bool registered = false;
      pollers().push_back([]() {
      auto q = internal::all_queues<Event>();
      auto l = internal::all_listeners<Event>();

      for (auto& eventqptr : q) {
        Queue<Event>& eventq = *eventqptr;
        for (auto& event : eventq) {
          for (auto& listenersptr : l) {
            Listeners<Event>& listeners = *listenersptr; //? lehet felesleges
            for (auto& listener : listeners) {
              if (cancelled<Event>()) {break;}
              listener(event);
            }
          }
        }
        eventq.clear();
      }
      });
      registered = true;
    }
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