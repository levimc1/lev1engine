#pragma once

#include <engine/core/taskm/task.hpp>
#include <engine/core/taskm/flow.hpp>
#include <stdexcept>
#include <cstring>

namespace taskm {

  // Helper
  namespace internal {

    template<typename... Ts>
    inline std::tuple<Ts...>& tuple_from_void(void* data) {
      return *reinterpret_cast<std::tuple<Ts...>*>(data);
    }
  }

  struct TaskBuilder {

    std::vector<std::function<void(TaskBuilder&, void*)>> functions;
    std::vector<std::vector<std::pair<void*, size_t>>> paramaters;

    size_t current = 0;

    std::string name;
    uint64_t id;

    internal::Task result_task;

    void operator<<(std::function<void(TaskBuilder&, void*)> func) {
      current++;
      functions.push_back(func);
    }

    template<typename T>
    TaskBuilder& operator,(const T& arg) {
      if (current -1 < 0) {throw std::runtime_error("Rosszul hazsnálod a buildert!");}
      paramaters.at(current - 1).push_back(arg, sizeof(arg)); 
    }
    

    void operator>>(TaskID& id) {
      for (auto& func : functions) {
        size_t total_size = 0;
        for (auto& [data, size] : paramaters[current - 1]) {total_size += size;}

        char* result = new char[total_size];
        
        size_t offset = 0;
        for (auto& [data, size] : paramaters[current - 1]) {std::memcpy(result + offset, data, size); offset += size;}

        func(*this, result);
        
        id.pos = add(result_task);
        name_table.insert_or_assign(name, id);
      }

      
    } 


    TaskBuilder& function(std::function<void()> func) {
      result_task.functions.push_back(func);
      return *this;
    }

    TaskBuilder& condition(std::function<bool()> func) {
      result_task.conditions.push_back(func);
      return *this;
    }
  };

  TaskBuilder begin(std::string name) {
    TaskBuilder b{};
    b.name = name;
    return b;
  }
}