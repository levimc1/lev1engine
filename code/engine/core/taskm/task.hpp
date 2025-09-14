#pragma once


#include <functional>
#include <stdexcept>
#include <vector>
#include <type_traits>


namespace taskm {

  struct TaskID {
    unsigned long long id;
    size_t pos;
  };


  namespace internal {

    // Wrapperek
    struct Condition {
      std::function<bool()> func;
    };
    struct ProcessF {
      std::function<void()> func;
    };
  }

  struct Task {
    TaskID id;
    std::vector<internal::ProcessF> process;
    std::vector<internal::Condition> conditions;
  };

  namespace internal {

    std::vector<Task> tasks;
    std::vector<Task*> next;

    template<typename Arg>
    void task_parser(Task& task, TaskID& id, Arg&& arg) {
      using T = std::decay_t<Arg>;
      if (constexpr std::is_same_v<T, Condition>) {
        task.conditions.push_back(std::forward<Arg>(arg));
      } else if (constexpr std::is_same_v<T, ProcessF>) {
        task.process.push_back(std::forward<Arg>(arg));
      } else {
        throw std::runtime_error("Rossz típus make_task-nak!");
      }
    }
  }

  template<typename... Args>
  Task make_task(unsigned int idname = 0, Args&&... args) {
    TaskID id{idname, internal::tasks.size()};
    Task task;
    (internal::task_parser(task, id, std::forward<Args>(args)),...);
    task.id = id;
    return Task;
  }

  // TODO: azok az izék. tudod.
  inline internal::Condition condition(std::function<bool()> f) {
    return internal::Condition{f};
  }
  inline internal::ProcessF process(std::function<void()> f) {
    return internal::ProcessF{f};
  }

}