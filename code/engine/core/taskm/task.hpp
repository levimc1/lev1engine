#pragma once


#include <functional>
#include <stdexcept>
#include <vector>
#include <type_traits>


namespace taskm {

  struct TaskID {
    unsigned long long id;
    size_t pos;

    bool operator==(const TaskID& other) const noexcept {
      return id == other.id && pos == other.pos;
    }
    struct Hash {
      std::size_t operator()(const TaskID& tid) const noexcept {
        return std::hash<unsigned long long>()(tid.id) ^ std::hash<size_t>()(tid.pos);
      }
    };
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
  }
  struct TaskAddress {
    size_t pos;
    bool valid = false;
    TaskAddress(size_t p) : pos(p), valid(true) {}
    TaskAddress(std::nullptr_t*): pos(0), valid(false) {}

    TaskAddress(TaskAddress&& other) noexcept : pos(other.pos), valid(other.valid) {
      other.valid = false;
    }

    TaskAddress& operator=(TaskAddress&& other) noexcept {
      if (this != &other) {
        pos = other.pos;
        valid = other.valid;
        other.valid = false;
      }
      return *this;
    }

    Task* get() {
      
      if (!valid) return nullptr;
      return &internal::tasks[pos];
    }
  };

  namespace internal {

    std::vector<TaskAddress> next;

    template<typename Arg>
    void task_parser(Task& task, TaskID& id, Arg&& arg) {
      using T = std::decay_t<Arg>;
      if constexpr (std::is_same_v<T, Condition>) {
        task.conditions.push_back(std::forward<Arg>(arg));
      } else if constexpr (std::is_same_v<T, ProcessF>) {
        task.process.push_back(std::forward<Arg>(arg));
      } else {
        throw std::runtime_error("Rossz típus make_task-nak!");
      }
    }
  }

  template<typename... Args>
  Task make_task(unsigned int idname = 0, Args&&... args) {
    TaskID id{idname, size_t(-1)};
    Task task;
    (internal::task_parser(task, id, std::forward<Args>(args)),...);
    task.id = id;
    return task;
  }

  // TODO: azok az izék. tudod.
  inline internal::Condition condition(std::function<bool()> f) {
    return internal::Condition{f};
  }
  inline internal::ProcessF process(std::function<void()> f) {
    return internal::ProcessF{f};
  }

}