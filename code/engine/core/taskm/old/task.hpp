#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>

namespace taskm {

  struct TaskID {
    uint64_t id;
    size_t pos;

  };

  namespace internal {

    struct Task {
      std::vector<std::function<bool()>> conditions; 
      std::vector<std::function<void()>> functions;
      TaskID id; 
      Task* ptr(Task* begin) {return begin + id.pos;};
    };
    
    std::vector<Task> tasks;
    void reorder() {
      for (int x = 0; x < tasks.size(); x++) {
        tasks.at(x).id.pos = x;
      }
    }
  }
  
  std::unordered_map<std::string, internal::Task*> name_table;
  TaskID idof(std::string name) {return name_table[name]->id;}
  size_t add(internal::Task task) {internal::tasks.push_back(task); return internal::tasks.size();}


} 