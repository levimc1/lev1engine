#pragma once

#include <vector>

namespace eventh {

  namespace internal {
    template<typename Event>
    bool& cancelled() {
      static bool cancelled = false;
      return cancelled;
    }

    std::vector<void(*)()> resetters;

  }

    template<typename Event>
    inline void cancel() {
      internal::resetters.push_back([](){
        internal::cancelled<Event>() = false;
      });
      internal::cancelled<Event>() = true;
    }

  template<typename Event>
  inline void set_cancelled(bool state) {
    if (state) {
     internal::resetters.push_back([](){
        internal::cancelled<Event>() = false;
      });
    }
    internal::cancelled<Event>() = state;
  }

}