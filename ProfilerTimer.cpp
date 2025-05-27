#include <chrono>
#include "ProfilerTimer.h"
#include "Profiler.h"

ProfilerTimer::ProfilerTimer (const std::string& name): m_name(name) {
  start();
}

ProfilerTimer::~ProfilerTimer () {
  end();
}

void ProfilerTimer::start () {
  std::chrono::time_point<std::chrono::high_resolution_clock> stp = std::chrono::high_resolution_clock::now();
  m_start = std::chrono::time_point_cast<std::chrono::microseconds>(stp).time_since_epoch().count();
}

void ProfilerTimer::end () {
  std::chrono::time_point<std::chrono::high_resolution_clock> etp = std::chrono::high_resolution_clock::now();
  m_end = std::chrono::time_point_cast<std::chrono::microseconds>(etp).time_since_epoch().count();

  ProfileResult r;
  r.name = m_name;
  r.start = m_start;
  r.end = m_end;

  Profiler::Instance().writeProfile(r);
}

