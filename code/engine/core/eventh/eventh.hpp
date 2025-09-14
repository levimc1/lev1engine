#pragma once

#include <vector>
#include <functional>
#include <unordered_map>

#include "contextm.hpp"

namespace eventh {

  template<typename Event>
  void internal::ensure_registered();

  template<typename Event, typename... Args>
  void emit(Args... args) {
    internal::ensure_registered<Event>();
    contextm::ctx_active.at(0).queue<Event>().emplace_back(std::forward<Args>(args)...);
  }

  template<typename Event, typename... Args>
  void emit(const EventhContext& ctx, Args... args) {
    internal::ensure_registered<Event>();
    ctx.queue<Event>().emplace_back(std::forward<Args>(args)...);
  }

  template<typename Event, typename... Args>
  void emit_now(Args... args) {

    internal::ensure_registered<Event>();
    Event event(std::forward<Args>(args)...);
    auto l = internal::all_listeners<Event>();
    for (auto& listenersptr : l) {
      Listeners<Event>& listeners = *listenersptr; //? lehet felesleges
      for (auto& listener : listeners) {
        listener(event);
      }
    }
  }

  void poll() {
    for (auto& poller : internal::pollers()) {
      poller();
    }
    for (auto& resetter : internal::resetters) {
      resetter();
    }
    internal::resetters.clear();
  }

  template<typename Event>
  void subscribe(void(*listener)(Event&)) {
    contextm::ctx_active.at(0).listeners<Event>().emplace_back(listener);
  }

  template<typename Event>
  void subscribe(const EventhContext& ctx, void(*listener)(Event&)) {
    ctx.listeners<Event>().emplace_back(listener);
  }

}