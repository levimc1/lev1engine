#pragma once

#include <thread>
#include <chrono>
#include <engine/core/taskm/builder.hpp>

namespace taskm {
  void init() {
    internal::tasks = {};
    internal::controllers = {}; // a default hozzáadása. 
  }
}