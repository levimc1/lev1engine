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
    bool running = true;
  }

  void init() {
    internal::controllers.push_back({&internal::repeat_controller});
    internal::controllers.push_back({&internal::schedule_controller});
    internal::running = true;
  }

  // írd újra
  void tick() {

    // controllerek futtatása. 
    for (auto* controller_ptr = internal::controllers.data();
       controller_ptr != internal::controllers.data() + internal::controllers.size();
       ++controller_ptr) { // ez asszem a helyes loop (ptr it)
      controller_ptr->func(); // controller structban lévő func meghívása. nincs segfault
    }
    internal::failed.clear(); // failed törlése a controllerek futása után

    for (size_t task_i = 0; // nexten, HELYES loop. next az Task* azaz task_ptr az Task**
       task_i < internal::next.size();
       task_i++) { // Helyes it loop

      auto& task = internal::next[task_i];

      if (!task.valid) {internal::running = false; return;};
      auto* task_ptr = task.get();  

      bool ready = true;
      for (auto& cond : task_ptr->conditions) { // normál loop.
        if (!cond.func()) { // Ha egy is hamis, mind hamise.
          ready = false;
          break;
        }
      }
      if (ready) { // Ha végül igaz lett
        for (auto* task_proc_ptr = task_ptr->process.data(); // a Task* nek a processén ptr loop
        task_proc_ptr != task_ptr->process.data() + task_ptr->process.size();
        ++task_proc_ptr) { // Helyes iterátor loop?
          task_proc_ptr->func(); // Ez egy ProcessF* most
        }
      } else {
        internal::failed.insert(task_ptr->id);
      }
    }
    
    internal::next.clear();
    internal::frame++;
  }

  void cleanup() {
    
  }

  void add(Task& task) {
    internal::tasks.push_back(task);
    task.id.pos = internal::tasks.size() - 1;
    std::cout << internal::tasks.size() - 1 << std::endl;
  }
}