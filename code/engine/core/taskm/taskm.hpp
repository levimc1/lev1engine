#pragma once

/*fontos: ennek és flow.hpp-nek kevés dolga van. Holnapra:
  Ide rakj valamit(init), akármit. flow.hpp-be ábrázolás a Controller-hez.
  És def_flow/
*/

#include <engine/core/taskm/flag.hpp>
#include <engine/core/taskm/flow.hpp>
#include <engine/core/taskm/def_flow/repeat.hpp>
#include <engine/core/taskm/def_flow/scheduler.hpp>

namespace taskm {
  
  namespace internal {

  }

  void init() {
    internal::controllers.push_back({&internal::repeat_controller});
    internal::controllers.push_back({&internal::schedule_controller});
  }

  void tick() {

    for (auto* controller_ptr = internal::controllers.data();
       controller_ptr != internal::controllers.data() + internal::controllers.size();
       ++controller_ptr) {
      controller_ptr->func();
    }
    internal::failed.clear();

    for (auto* task_ptr = internal::next.data();
       task_ptr != internal::next.data() + internal::next.size();
       ++task_ptr) {
      if (!*task_ptr) return;
      bool ready = true;
      for (auto& cond : (*task_ptr)->conditions) {
        if (!cond.func()) {
          ready = false;
          break;
        }
      }
      if (ready) {
        for (auto* task_proc_ptr = (*task_ptr)->process.data();
        task_proc_ptr != (*task_ptr)->process.data() + (*task_ptr)->process.size();
        ++task_proc_ptr) {
          task_proc_ptr->func();
        }
      } else {
        internal::failed.insert((*task_ptr)->id);
      }
      internal::next.clear();
    }
    
    internal::frame++;
  }

  void cleanup() {
    
  }

  void add(Task task) {
    internal::tasks.push_back(task);
  }
}