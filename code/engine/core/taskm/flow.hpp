#pragma once

#include <engine/core/taskm/task.hpp>
#include <unordered_set>
// TODO: ábrázolás, mivel ez nagyon kicsi eddig.

namespace taskm {

  namespace internal {

    unsigned long long frame = 0;
       
    // Wrapper
    struct Controller {
      std::function<void()> func;
    };

    std::vector<Controller> controllers;
    std::unordered_set<TaskID> failed;
    std::vector<Task*> next;
    
  }

  void controller(std::function<void()> func) {
    internal::controllers.push_back({func});
  }

  inline void run(TaskID id) {
    if (id.pos < internal::tasks.size()) {
      internal::next.push_back(&internal::tasks[id.pos]);
    }
  }

  void end() {
    internal::next.push_back(nullptr);
  }
}
