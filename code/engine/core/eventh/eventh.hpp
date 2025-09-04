#pragma once

#include <vector>
#include <functional>
#include <unordered_map>

#include "context.hpp"

namespace eventh {
  struct EventhContext;
  static EventhContext context;

  template<typename Event>
  void flush() {
    auto& queue = context.queue<Event>();
    auto& listeners = context.listeners<Event>();

    for (auto& event : queue) {
      for (auto& listener : listeners) {
        listener(event);
      }
    }
    queue.clear();
  }

  template<typename Event, typename... Args>
  void emit(Args... args) {
    context.ensure_registered<Event>();
    context.queue<Event>().emplace_back(std::forward<Args>(args)...);
  }

  template<typename Event, typename... Args>
  void emit_now(Args... args) {

    context.ensure_registered<Event>();
    Event event(std::forward<Args>(args)...);
    for (auto& listener : context.listeners<Event>) {
      listener(event);
    }
  }

  void poll() {
    for (auto& poller : context.pollers()) {
      poller();
    }
  }

  template<typename Event>
  void subscribe(void(*listener)(Event&)) {
    context.listeners<Event>().emplace_back(listener);
  }

  static void use(const EventhContext& ctx) {
    context = ctx;
  }
  static EventhContext init() {
    EventhContext ctx = EventhContext();
    use(ctx);
    return ctx;
  }
}