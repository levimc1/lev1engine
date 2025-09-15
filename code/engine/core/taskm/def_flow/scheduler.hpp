#pragma once

#include <chrono>
#include <engine/core/taskm/flow.hpp>
#include <engine/core/taskm/task.hpp>
#include <algorithm>
#include <map>

//! SZAR



namespace taskm {

  // TODO: megoldás ahoz is ha nincsen delta time és ezek, csak futás.
  constexpr bool wait_for_frame = true;

  // Todo: import from game
  const std::chrono::milliseconds FRAME_TIME{16};  
  uint64_t frame_c;
  std::chrono::steady_clock::time_point start_time;

  struct TimePoint {
    std::chrono::steady_clock::time_point time;

    bool operator<(const TimePoint& other) const noexcept {
      return time < other.time;
    }

    bool operator==(const TimePoint& other) const noexcept {
      return time == other.time;
    }

    struct Hash {
      std::size_t operator()(const TimePoint& tp) const noexcept {
        return std::hash<int64_t>()(
          std::chrono::duration_cast<std::chrono::nanoseconds>(
            tp.time.time_since_epoch()
          ).count()
        );
      }
    };
  };

  TimePoint timepoint(std::chrono::steady_clock::time_point time) {return {time};}

  namespace internal {
    std::unordered_map<uint64_t, std::vector<std::pair<TaskID, TimePoint>>> frame_scheduled;
    std::map<TimePoint, std::vector<TaskID>> time_scheduled;

    void schedule_controller() {
      if (wait_for_frame) {
        auto it = frame_scheduled.find(frame_c);
        if (it != frame_scheduled.end()) {
          for (auto& pair : it->second) {
            run(pair.first);
          }
          frame_scheduled.erase(it);
        }
        frame_c++;
      } else {
        // Idő alapú. Autimatikusan rendezett. Ha az idő kevesebb, akkor fut és töröl.
        for (auto& entry : time_scheduled) {
          if (entry.first.time <= std::chrono::steady_clock::now()) {
            for (auto& id : entry.second) {
              run(id);
            } 
          time_scheduled.erase(entry.first);
          } else {
            break;
          }
        }
      }
    }
  }

  void schedule(TaskID id, TimePoint when) {
    if (wait_for_frame) {
      int64_t frame_i = when.time.time_since_epoch().count() - start_time.time_since_epoch().count();
      if (frame_i < 0) {return;}
      uint64_t frame = frame_i / FRAME_TIME.count();
      if (frame < frame_c) {return;}
      auto it = internal::frame_scheduled.find(frame);
      if (it == internal::frame_scheduled.end()) {
        internal::frame_scheduled[frame] = {{id, when}};
      } else {
        it->second.push_back({id, when});
      }
    } else {
      if (when.time < start_time) {return;}
      if (when.time < std::chrono::steady_clock::now()) {return;}
      auto it = internal::time_scheduled.find(when);
      if (it == internal::time_scheduled.end()) {
        internal::time_scheduled[when] = {{id}};
      } else {
        it->second.push_back(id);
      }
    }
  }
  void delay(TaskID id, std::chrono::milliseconds delay) {
    schedule(id, timepoint(std::chrono::steady_clock::now() + delay));
  }
}