#pragma once

#include <engine/core/taskm/task.hpp>
#include <engine/core/taskm/flow.hpp>
#include <algorithm>

namespace taskm {

  namespace internal {
    std::vector<TaskID> repeating;
    void repeat_controller() {
      for (auto* repeating_ptr = repeating.data();
        repeating_ptr != internal::repeating.data() + internal::repeating.size();
        ++repeating_ptr
      ) {
        if (failed.find(*repeating_ptr) == failed.end()) {
          run(*repeating_ptr);
        } else {
          repeating.erase(std::find(repeating.begin(), repeating.end(), *repeating_ptr));
        }
      }
    }
  }

  void repeat(TaskID id) {
    internal::repeating.push_back(id);
  }
}