#pragma once

#include <functional>
#include <vector>
#include <engine/core/taskm/task.hpp>
#include <list>
#include <thread>
#include <unordered_map>
#include <deque>


using TimeStamp = uint64_t;

namespace taskm {

  namespace internal {
    std::vector<std::function<void(std::vector<Task*>)>> controllers;
  
    // Normal mode, miután mi.
    std::deque<TaskID> ordered;

    // Csak futtatja az összest
    std::vector<TaskID> run_all;

    // Idő
    std::unordered_map<TimeStamp, std::vector<TaskID>> scheduled;

    // Repeat until false
    std::vector<TaskID> repeat_cond;
  }

  // TODO: default flow izé

  inline std::chrono::system_clock::time_point start;

  TimeStamp time(std::chrono::system_clock::time_point real);

  void flow_control(std::function<void(std::vector<internal::Task*>)> func) {
    internal::controllers.push_back(func);
  }

  void delay(TaskID id, unsigned int frames); // schedule extrákkal
  void run(TaskID id);
  void schedule(TaskID id, TimeStamp timestamp, std::thread t); 
  void repeat(TaskID id, unsigned int everyxframe, std::thread t);   
  void after(TaskID after, TaskID run) {

  }
  void before(TaskID before, TaskID run) {

  }

}