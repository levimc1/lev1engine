#pragma once

#include <vector>

namespace eventh {

  template<typename Event>
  bool& cancelled() {
    static bool cancelled = false;
    return cancelled;
  }

  std::vector<void(*)()> resetters;

  template<typename Event>
  inline void cancel() {
    resetters.push_back([](){
      cancelled<Event>() = false;
    });
    cancelled<Event>() = true;
  }

  template<typename Event>
  inline void set_cancelled(bool state) {
    if (state) {
     resetters.push_back([](){
        cancelled<Event>() = false;
      });
    }
    cancelled<Event>() = state;
  }

}